#include "msganalyzer.h"

#define MAX_READ_SIZE 1024
#define Timeout 1000
#define BEGIN "<?BEGIN?>"
#define END "<?END?>"

MsgAnalyzer::MsgAnalyzer(ClientDataBase* db){
    this->db=db;
}

void MsgAnalyzer::readMessage(QTcpSocket* socket){
    if (socket->isReadable()) {
        QElapsedTimer timer;

        timer.start();
        QByteArray receivedData;
        bool foundBegin = false;
        while (true) {
            QByteArray buf = socket->read(MAX_READ_SIZE);
            if(buf.length()!=0)
                timer.restart();
            else
                QThread::msleep(50);
            if(timer.hasExpired(Timeout))
                sendError(socket,"timeout");
            receivedData.append(buf);
            if (!foundBegin) {
                int beginIndex = receivedData.indexOf(BEGIN);
                if (beginIndex != -1) {
                    receivedData.remove(0, beginIndex + QString(BEGIN).length());
                    foundBegin = true;
                }else{
                    qDebug() << "invalid message:" << receivedData;
                    sendError(socket,"invalid input");
                    return;
                }
            }
            if (foundBegin && receivedData.contains(END)) {
                int endIndex = receivedData.indexOf(END);
                QByteArray message = receivedData.mid(0, endIndex);
                anaylze(socket,message);
                qDebug() << "Received message:" << message;
                receivedData.remove(0, endIndex + strlen(END));
                foundBegin = false; // Reset for the next message
                if (receivedData.length()==0)
                    break;
            }
        }
    }
}

void MsgAnalyzer::sendError(QTcpSocket* socket,QString error){
    QJsonObject json;
    json["function"] = "error";
    json["data"]=error;
    QJsonDocument jsonDoc(json);
    QString jsonString = "<?BEGIN?>"+jsonDoc.toJson(QJsonDocument::Compact)+"<?END?>";
    socket->write(jsonString.toUtf8());
}

void MsgAnalyzer::storeIntoDatabase(QJsonObject information){

}

void MsgAnalyzer::anaylze(QTcpSocket* socket,QString message){
    QJsonDocument document=QJsonDocument::fromJson(message.toUtf8());
    if (!document.isNull()) {
        QJsonObject information=document.object();
        try{
            if(information["function"].toString()=="login"){
                if(information["data"].toString()=="success")
                    emit successLogin();
                else
                    QMessageBox::information(nullptr, "Information", information["data"].toString());
            }

        } catch (const std::exception &e) {
            qDebug() << "Exception caught:" << e.what(); // 捕获并处理异常
            sendError(socket,e.what());
        }
    }else
        sendError(socket,"improper json");
}
