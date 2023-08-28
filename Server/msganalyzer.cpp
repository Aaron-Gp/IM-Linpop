#include "msganalyzer.h"

MsgAnalyzer::MsgAnalyzer(Msg msg,QList<Client>* m_clients,QObject* parent):db(DataBase(PATH)){
    this->msg=msg;
    this->m_clients=m_clients;
}

MsgAnalyzer::~MsgAnalyzer(){
    db.close();
}

void MsgAnalyzer::storeIntoDatabase(QJsonObject information){
    long long timeStamp=QDateTime::currentMSecsSinceEpoch();
    db.exec(QString("insert into record(sender_id,receiver_id,timestamp,type,data) values(%1,%2,%3,'%4','%5')").arg
                (information["sender"].toInt()).arg(information["receiver"].toInt()).arg(timeStamp).arg
                (information["type"].toString()).arg(information["data"].toString()));
}

void MsgAnalyzer::sendError(QTcpSocket* socket,QString error){
    QJsonObject json;
    json["function"] = "error";
    json["data"]=error;
    QJsonDocument jsonDoc(json);
    QString jsonString = "<?BEGIN?>"+jsonDoc.toJson(QJsonDocument::Compact)+"<?END?>";
    emit send(socket,Msg(socket,msg.socket->peerPort(), socket->peerAddress().toString(), jsonString));
    //TcpServer::SendMsgTo(socket,Msg(socket,msg.socket->peerPort(), socket->peerAddress().toString(), jsonString));
}

void MsgAnalyzer::sendResult(QTcpSocket* socket,QString function,QString data){
    QJsonObject json;
    json["function"] = function;
    json["data"]=data;
    QJsonDocument jsonDoc(json);
    QString jsonString = "<?BEGIN?>"+jsonDoc.toJson(QJsonDocument::Compact)+"<?END?>";
    emit send(socket,Msg(socket,msg.socket->peerPort(), socket->peerAddress().toString(), jsonString));
    //TcpServer::SendMsgTo(socket,Msg(socket,msg.socket->peerPort(), socket->peerAddress().toString(), jsonString));
}

void MsgAnalyzer::sendJson(QTcpSocket* socket,QJsonObject json){
    QJsonDocument jsonDoc(json);
    QString jsonString = "<?BEGIN?>"+jsonDoc.toJson(QJsonDocument::Compact)+"<?END?>";
    emit send(socket,Msg(socket,msg.socket->peerPort(), socket->peerAddress().toString(), jsonString));
    //TcpServer::SendMsgTo(socket,Msg(socket,msg.socket->peerPort(), socket->peerAddress().toString(), jsonString));
}

void MsgAnalyzer::anaylze(){
    QJsonDocument document=QJsonDocument::fromJson(msg.buf.toUtf8());
    if (!document.isNull()) {
        QJsonObject information=document.object();
        try{
            if(information["function"].toString()=="information"){
                if(information["length"]!=information["data"].toString().length())
                    sendError(msg.socket,"incomplete data");
                else{
                    int receiver_id=information["receiver"].toInt();
                    db.exec(QString("SELECT is_online FROM user WHERE user_id=%1").arg(receiver_id));
                    db.query.next();
                    bool is_online=db.query.value(0).toBool();
                    if(!is_online)
                        storeIntoDatabase(information);
                    else{
                        /*
                        if(!tryConnect(ip)){
                            db.exec(QString("UPDATE user SET is_online=false WHERE user_id=%1").arg(receiver_id));
                            storeIntoDatabase(information);
                        }else{
                            long long timeStamp=QDateTime::currentMSecsSinceEpoch();
                            information["timeStamp"]=timeStamp;
                            QJsonDocument jsonDoc(information);
                            QString jsonString = "<?BEGIN?>"+jsonDoc.toJson(QJsonDocument::Compact)+"<?END?>";
                            client->write(jsonString.toUtf8());
                        }
                        */
                        int flag=0;
                        for (QList<Client>::iterator iter = m_clients->begin(); iter != m_clients->end(); iter++){
                            if (iter->id==information["receiver"].toInt()){
                                if(iter->sock->state()!=QAbstractSocket::UnconnectedState){
                                    long long timeStamp=QDateTime::currentMSecsSinceEpoch();
                                    information["timeStamp"]=timeStamp;
                                    sendJson(iter->sock,information);
                                    flag=1;
                                    break;
                                }
                            }
                        }
                        if(!flag){
                            db.exec(QString("UPDATE user SET is_online=false WHERE user_id=%1").arg(receiver_id));
                            storeIntoDatabase(information);
                        }
                    }
                }
            }
            if(information["function"].toString()=="login_in"){
                db.exec(QString("SELECT user_id FROM user WHERE user_id=%1").arg(information["sender"].toInt()));
                if(!db.query.next())
                    sendResult(msg.socket,"result","account_not_found");
                else{
                    db.exec(QString("SELECT user_id FROM user WHERE user_id=%1 and password='%2'").arg(information["sender"].toInt()).arg(information["password"].toString()));
                    if(!db.query.next())
                        sendResult(msg.socket,"result","error_password");
                    else{
                        for (QList<Client>::iterator iter = m_clients->begin(); iter != m_clients->end(); iter++)
                            if (msg.socket==iter->sock)
                                iter->id=information["sender"].toInt();
                        sendResult(msg.socket,"result","success");
                        db.exec(QString("update user set is_online=true where user_id=%1").arg(information["sender"].toInt()));
                        db.exec(QString("SELECT * FROM record WHERE receiver_id=%1").arg(information["sender"].toInt()));
                        while (db.query.next()) {
                            QJsonObject jsonObject;
                            jsonObject["function"] = "information";
                            jsonObject["sender"] = db.query.value("sender_id").toInt();
                            jsonObject["receiver"] = db.query.value("receiver_id").toInt();
                            jsonObject["timestamp"] = db.query.value("timestamp").toString();
                            jsonObject["type"] = db.query.value("type").toString();
                            jsonObject["data"] = db.query.value("data").toString();
                            sendJson(msg.socket,jsonObject);
                        }
                        db.exec(QString("delete FROM record WHERE receiver_id=%1").arg(information["sender"].toInt()));
                    }
                }
            }
        } catch (const std::exception &e) {
            qDebug() << "Exception caught:" << e.what(); // 捕获并处理异常
            sendError(msg.socket,"invalid_input");
        }
    }else
        sendError(msg.socket,"improper_json");
    emit finish();
}
