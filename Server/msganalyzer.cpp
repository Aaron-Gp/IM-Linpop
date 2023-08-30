#include "msganalyzer.h"


MsgAnalyzer::MsgAnalyzer(Msg msg,QList<Client>* m_clients,QObject* parent):db(DataBase(PATH)){
    this->msg=msg;
    this->m_clients=m_clients;
}

MsgAnalyzer::~MsgAnalyzer(){
    db.close();
}

void MsgAnalyzer::storeIntoDatabase(QJsonObject information){
    //int timestamp=QDateTime::currentMSecsSinceEpoch();
    //information["timestamp"]=timestamp;
    if(information["type"]=="file"){
        FileManager::ToFlie(information["data"].toString(),FILE_PATH,information["fileName"].toString(),information["sender"].toInt(),information["receiver"].toInt(),information["timestamp"].toInt());
        information["data"]=information["fileName"];
    }
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
                if(information["size"]!=information["data"].toString().length())
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
                                    //int timestamp=QDateTime::currentMSecsSinceEpoch();
                                    //information["timestamp"]=timestamp;
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
                switch(db.isUserAccountCorrect(information["sender"].toInt(),information["password"].toString(),msg.socket->peerAddress().toString())){
                case 1:
                    sendResult(msg.socket,"login","account not found");
                    break;
                case 2:
                    sendResult(msg.socket,"login","incorrect password");
                    break;
                case 3:
                    for (QList<Client>::iterator iter = m_clients->begin(); iter != m_clients->end(); iter++)
                        if (msg.socket==iter->sock)
                            iter->id=information["sender"].toInt();
                    //sendResult(msg.socket,"login","success");//在isUserAccountCorrect中已更改登录状态
                    QJsonObject json;
                    json["function"] = "login";
                    json["data"]="success";
                    json["id"]=information["sender"].toInt();
                    db.exec(QString("select name from user where id=%1").arg(information["sender"].toInt()));
                    db.query.next();
                    json["name"]=db.query.value("name").toString();
                    QJsonDocument jsonDoc(json);
                    QString jsonString = "<?BEGIN?>"+jsonDoc.toJson(QJsonDocument::Compact)+"<?END?>";
                    emit send(msg.socket,Msg(msg.socket,msg.socket->peerPort(), msg.socket->peerAddress().toString(), jsonString));
                    QList<QJsonObject> jsonList;
                    db.getMessage(information["sender"].toInt(),jsonList);
                    for (QList<QJsonObject>::iterator iter = jsonList.begin(); iter != jsonList.end(); iter++){
                        if((*iter)["type"]=="file"){
                            (*iter)["fileName"]=(*iter)["data"];
                            (*iter)["data"]=FileManager::ToString(FILE_PATH,(*iter)["fileName"].toString(),(*iter)["sender"].toInt(),(*iter)["receiver"].toInt(),(*iter)["timestamp"].toInt());
                            FileManager::deleteFile(FILE_PATH,(*iter)["fileName"].toString(),(*iter)["sender"].toInt(),(*iter)["receiver"].toInt(),(*iter)["timestamp"].toInt());
                        }
                        (*iter)["size"]=(*iter)["data"].toString().length();
                        sendJson(msg.socket,*iter);
                    }
                    db.deleteMessage(information["sender"].toInt());
                }
            }
        } catch (const std::exception &e) {
            qDebug() << "Exception caught:" << e.what(); // 捕获并处理异常
            sendError(msg.socket,e.what());
        }
    }else
        sendError(msg.socket,"improper json");
    emit finish();
}
