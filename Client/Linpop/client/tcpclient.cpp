#include "tcpclient.h"
#include <QMessageBox>
#include <QDateTime>
#include <QThread>

TcpClient::TcpClient(QWidget *parent) : QWidget(parent)
{
    m_profile = ProfileManager::getInstance();
    db=new ClientDataBase;
    analyzer = new MsgAnalyzer(db);
    {//用于和服务器连接
        m_server = new QTcpSocket();   //实例化tcpClient
        m_server->connectToHost(SERVER, PORT);
        bool ok = m_server->waitForConnected(1000);
        if (!ok)
            MYLOG<<"fail to connect server";
        else
            MYLOG<<"success to connect server";
        connect(m_server, &QTcpSocket::readyRead, analyzer,[&](){
            analyzer->readMessage(m_server);
        });
        connect(m_server, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), [&](){
            m_server->disconnectFromHost();
            QMessageBox msgBox;
            msgBox.setText(tr("disconnect with server"));
            msgBox.exec();
        });
    }
}

void TcpClient::newConnection(QString ip)
{
    //初始化TCP客户端
    QTcpSocket *tcpClient = new QTcpSocket();   //实例化tcpClient
    tcpClient->connectToHost(ip, 8000);
    bool ok = tcpClient->waitForConnected(1000);
    if (ok)
    {
        QString ip = tcpClient->peerAddress().toString()+":"+QString::number(tcpClient->peerPort());
        MYLOG<<"connect to "+ip;
        m_tcpClient.insert(ip, tcpClient); // 将新的连接加入连接池中
        bool exist = m_profile->m_contact.contains(ip);
        if(!exist){
            MYLOG<<"new contact "+ip;
            m_profile->m_contact.append(ip);
            profile newContact;
            newContact.ip = ip;
            m_profile->m_contactProfile.append(newContact);
            m_profile->m_chatList.insert(ip, Message());
        }
        MYLOG<<"send hello message";
        QString data = "hello from client!";
        tcpClient->write(data.toUtf8());
    }else{
        MYLOG<<"fail to connect "<<ip;
    }
    connect(tcpClient, &QTcpSocket::readyRead, [=](){
        MYLOG<<"readyRead";
        QByteArray buffer = tcpClient->readAll();
        MYLOG<<"buffer right?";
        QString ip = tcpClient->peerAddress().toString()+":"+QString::number(tcpClient->peerPort());
        MYLOG<<"new buffer from"<<ip;
        if(!buffer.isEmpty())
        {
            MYLOG<<ip<<" : "<<QString::fromUtf8(buffer);
            message msg;
            msg.ip=ip;
            msg.time= QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
            msg.msg = QString::fromUtf8(buffer);
            m_profile->m_chatList[ip].append(msg);
            MYLOG<<"append into chatlist";
            emit appendMsg(ip, msg); // 发送信号
        }
    });
    connect(tcpClient, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), [&](){
        tcpClient->disconnectFromHost();
        QMessageBox msgBox;
        msgBox.setText(tr("failed to connect server because %1").arg(tcpClient->errorString()));
        msgBox.exec();
    });
}

void TcpClient::sendData(QString ip, QString msg){

}

void TcpClient::sendToServer(QString msg){
    m_server->write(msg.toUtf8());
}

TcpClient::~TcpClient()
{
    for(auto iter = m_tcpClient.begin(); iter != m_tcpClient.end(); ++iter)
    {
        qDebug()<< "disconnect " << iter.key();
        iter.value()->abort();
    }
}
