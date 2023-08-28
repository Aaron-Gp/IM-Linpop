#include "sockettask.h"

#define BEGIN "<?BEGIN?>"
#define END "<?END?>"
#define MAX_READ_SIZE 1024
#define Timeout 1000
SocketTask::SocketTask(QTcpSocket *socket,QObject* m_pObj){
    this->socket=socket;
    this->m_pObj=m_pObj;
}

/*
void SocketTask::run(){
    if (socket->isReadable())
    {
        QByteArray buf = socket->readAll();
        if (!buf.isEmpty()){
            QMetaObject::invokeMethod(m_pObj, "handleNewMessage",
                                      Q_ARG(QTcpSocket*, socket),
                                      Q_ARG(Msg, Msg(socket,socket->peerPort(), socket->peerAddress().toString(), buf)));
            qDebug()<<buf;
        }
    }
}
*/

void SocketTask::run(){
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
            if(timer.hasExpired(Timeout)){
                QMetaObject::invokeMethod(m_pObj, "SendMsg",
                                          Q_ARG(QTcpSocket*, socket),
                                          Q_ARG(Msg,Msg(socket,socket->peerPort(), socket->peerAddress().toString(), "time out")));
                QMetaObject::invokeMethod(m_pObj, "reconnect",
                                          Q_ARG(QTcpSocket*, socket));
                return;
            }else{

            }
            receivedData.append(buf);
            if (!foundBegin) {
                int beginIndex = receivedData.indexOf(BEGIN);
                if (beginIndex != -1) {
                    receivedData.remove(0, beginIndex + QString(BEGIN).length());
                    foundBegin = true;
                }else{
                    //throw std::runtime_error("invalid information");
                    qDebug() << "invalid message:" << receivedData;
                    QMetaObject::invokeMethod(m_pObj, "SendMsg",
                                              Q_ARG(QTcpSocket*, socket),
                                              Q_ARG(Msg,Msg(socket,socket->peerPort(), socket->peerAddress().toString(), "invalid input")));
                    QMetaObject::invokeMethod(m_pObj, "reconnect",
                                              Q_ARG(QTcpSocket*, socket));
                    return;
                }
            }
            if (foundBegin && receivedData.contains(END)) {
                int endIndex = receivedData.indexOf(END);
                QByteArray message = receivedData.mid(0, endIndex);

                QMetaObject::invokeMethod(m_pObj, "handleNewMessage",
                                          Q_ARG(QTcpSocket*, socket),
                                          Q_ARG(Msg, Msg(socket, socket->peerPort(), socket->peerAddress().toString(), message)));
                qDebug() << "Received message:" << message;
                receivedData.remove(0, endIndex + strlen(END));
                foundBegin = false; // Reset for the next message
                if (receivedData.length()==0) {
                    break;
                }
            }
        }
    }
}

