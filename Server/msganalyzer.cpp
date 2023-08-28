#include "msganalyzer.h"

MsgAnalyzer::MsgAnalyzer(Msg msg,QList<Client>* m_clients,QObject* parent):db(DataBase(PATH)){
    this->msg=msg;
    this->m_clients=m_clients;
}

MsgAnalyzer::~MsgAnalyzer(){
    db.close();
}

void MsgAnalyzer::storeIntoDatabase(QJsonObject information){
    int timeStamp=QDateTime::currentMSecsSinceEpoch();
    information["timeStamp"]=timeStamp;
    db.addMessage(information);
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
                    if(!db.isUserOnline(receiver_id))
                        storeIntoDatabase(information);
                    else{
                        int flag=0;
                        for (QList<Client>::iterator iter = m_clients->begin(); iter != m_clients->end(); iter++){//找到对应id的socket
                            if (iter->id==information["receiver"].toInt()){
                                if(iter->sock->state()!=QAbstractSocket::UnconnectedState){
                                    int timeStamp=QDateTime::currentMSecsSinceEpoch();
                                    information["timeStamp"]=timeStamp;
                                    sendJson(iter->sock,information);
                                    flag=1;
                                    break;
                                }
                            }
                        }
                        if(!flag){
                            db.setUserAccountOffline(receiver_id);
                            storeIntoDatabase(information);
                        }
                    }
                }
            }
            if(information["function"].toString()=="login"){
                switch(db.isUserAccountCorrect(information["sender"].toInt(),information["password"].toString())){
                case 1:
                    sendResult(msg.socket,"result","account not found");
                    break;
                case 2:
                    sendResult(msg.socket,"result","incorrect password");
                    break;
                case 3:
                    for (QList<Client>::iterator iter = m_clients->begin(); iter != m_clients->end(); iter++)
                        if (msg.socket==iter->sock)
                            iter->id=information["sender"].toInt();
                    sendResult(msg.socket,"result","success");//在isUserAccountCorrect中已更改登录状态
                    QList<QJsonObject> jsonList;
                    db.getMessage(information["sender"].toInt(),jsonList);
                    for (QList<QJsonObject>::iterator iter = jsonList.begin(); iter != jsonList.end(); iter++)
                        sendJson(msg.socket,*iter);
                    db.deleteMessage(information["sender"].toInt());
                }
            }
        } catch (const std::exception &e) {
            qDebug() << "Exception caught:" << e.what(); // 捕获并处理异常
            sendError(msg.socket,e.what());
        }
    }else
        sendError(msg.socket,"improper_json");
    emit finish();
}
