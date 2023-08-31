#include "msganalyzer.h"

#define MAX_READ_SIZE 1024
#define Timeout 1000
#define BEGIN "<?BEGIN?>"
#define END "<?END?>"

#include <QMutex>

MsgAnalyzer* MsgAnalyzer::self = NULL;

MsgAnalyzer *MsgAnalyzer::getInstance()
{
    if (self == NULL) {
        //防止多进程同时调用创建2个对象的问题
        //原理是因为是静态变量只会定义一次
        static QMutex mutex;
        //上锁  QMutexLocker会锁住mutex，当QMutexLocker 被释放的时候会自动解锁
        //locker是局部变量，所以getInstance（）函数结束之后会自动解锁
        QMutexLocker locker(&mutex);
        //在判断一次,防止其它进程抢先new过了
        if (self == NULL) {
            self = new MsgAnalyzer;
        }
    }
    return self; //返回指针
}

MsgAnalyzer::MsgAnalyzer(){
    m_profile = ProfileManager::getInstance();
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

void MsgAnalyzer::anaylze(QTcpSocket* socket,QString message){
    QJsonDocument document=QJsonDocument::fromJson(message.toUtf8());
    if (!document.isNull()) {
        QJsonObject information=document.object();
        try{
            if(information["function"].toString()=="login"){
                if(information["data"].toString()=="success"){
                    emit successLogin();
                }
                else
                    QMessageBox::information(nullptr, "Information", information["data"].toString());
            }
            if(information["function"].toString()=="information"){
                if(information["size"]!=information["data"].toString().length())
                    sendError(socket,"incomplete data");
                else{
                    information["id"]=QString::number(information["sender"].toInt());
                    information["active"]=false;

                    emit storeIntoDatabase(information);
                }
            }
        } catch (const std::exception &e) {
            qDebug() << "Exception caught:" << e.what(); // 捕获并处理异常
            sendError(socket,e.what());
        }
    }else
        sendError(socket,"improper json");
}

void MsgAnalyzer::sendMessage(QTcpSocket *socket, QString function, message *msg = nullptr)
{
    QJsonObject obj;
    obj["function"]=function;
    if(function=="init"){
        if(msg!=nullptr){
            obj["time"]=msg->time;
            obj["msg"]=msg->msg;
        }
        obj["id"]=m_profile->m_id;
        QByteArray arr_base64 = m_profile->m_avatar.toUtf8();
        QByteArray ba = QByteArray::fromBase64(arr_base64);
        obj["avatar"]=QJsonValue::fromVariant(ba);
    }else if(function=="file"){
        obj["msg"]=msg->msg; // 文件信息
    }else if(function=="message"){
        obj["time"]=msg->time;
        obj["id"]=m_profile->m_id;
        obj["msg"]=msg->msg;
        obj["isSender"]=0;
    }else{
        MYLOG<<"send with wrong function";
        return;
    }
    QJsonDocument json;
    json.setObject(obj);
    QByteArray byteArray = json.toJson(QJsonDocument::Compact);
    MYLOG<<"send: "<<byteArray;
    socket->write(byteArray);
}

void MsgAnalyzer::receiveMessage(QTcpSocket *socket, QByteArray msg, bool isServer)
{
    MYLOG<<QString::fromUtf8(msg);
    QString ip;
    if(isServer)
        ip = socket->peerAddress().toString().split("::ffff:")[1]+":"+QString::number(socket->peerPort());
    else
        ip = socket->peerAddress().toString()+":"+QString::number(socket->peerPort());
    QJsonParseError jsonError;
    QJsonObject obj;
    QJsonDocument doucment = QJsonDocument::fromJson(msg, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {  // 解析未发生错误
        obj = doucment.object();
    }else{
        MYLOG<<"parse error: "<< jsonError.error;
        return;
    }

    QString id;

    if (obj.contains("id")) {
        id = obj.value("id").toString();
    }else{
        MYLOG<<"receive no id";
        return;
    }

    if (obj.contains("function")) {
        QString value = obj.value("function").toString();
        if(value == "init"){ // 初始化

            MYLOG<<"contact has: "<<m_profile->m_contact;
            if(!m_profile->m_contact.contains(id)){ // 新联系人
                m_profile->m_contactProfile.insert(id, profile());
                m_profile->m_chatList.insert(id,Message());
            }

            m_profile->m_contactProfile[id].id = id;
            m_profile->m_contactProfile[id].ip = ip;
            QByteArray avatar = obj.value("avatar").toVariant().toByteArray();
            QByteArray ba2 = avatar.toBase64();
            QString b64str = QString::fromUtf8(ba2);
            m_profile->m_contactProfile[id].avatar = b64str;
            MYLOG<<b64str;
            if(!m_profile->m_contact.contains(id)){
                m_profile->m_contact.append(id); // 到这里才能添加
                emit m_profile->addContact();
                MYLOG <<"new contact: id=" <<id;
            }else{
                emit m_profile->updateListBar();
                MYLOG<<"contact had";
            }
        }else if (value == "message"){
            MYLOG<<"receive message from id "<<id;
            message msg;
            msg.id = id;
            msg.msg = obj.value("msg").toString();
            msg.time = obj.value("time").toString();
            msg.isSender = 0;
            emit m_profile->appendMsg(id, msg);
        }
    }
}
