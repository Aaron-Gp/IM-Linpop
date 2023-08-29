#include "tcpserver.h"
#include <QDateTime>

TcpServer::TcpServer(QWidget *parent) : QWidget(parent)
{
    tcpServer = new QTcpServer();
    m_profile = ProfileManager::getInstance();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));
}

TcpServer::~TcpServer()
{
    closeAll();
}

void TcpServer::closeAll()
{
    for(auto iter=m_tcpClient.begin(); iter!=m_tcpClient.end(); ++iter)//断开所有连接
    {
        QTcpSocket *tcpClien = iter.value();
        tcpClien->disconnectFromHost();
        bool ok = tcpClien->waitForDisconnected(1000);
        if(!ok)
        {
            // 处理异常
        }
        m_tcpClient.remove(iter.key());  //从保存的客户端列表中取去除
    }
    tcpServer->close();     //不再监听端口
}

void TcpServer::NewConnectionSlot()
{
    currentClient = tcpServer->nextPendingConnection();
    QString ip = currentClient->peerAddress().toString().split("::ffff:")[1]+currentClient->peerPort();
    MYLOG<<"new connection from "<<ip;

    connect(currentClient, &QTcpSocket::readyRead, [&](){
        QByteArray buffer = currentClient->readAll();
        if(!buffer.isEmpty()){
            QString ip = currentClient->peerAddress().toString().split("::ffff:")[1]+currentClient->peerPort();
            MYLOG<<ip<<" : "<<QString::fromUtf8(buffer);
            message msg;
            msg.msg = QString::fromUtf8(buffer);
            msg.ip=ip;
            msg.time=QString::number(QDateTime::currentDateTime().toTime_t());
            emit appendMsg(ip, msg);
        }
    });

    connect(currentClient, &QTcpSocket::disconnected, [&](){
        if(currentClient->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在tcpClient列表中的客户端信息
            QString ip = currentClient->peerAddress().toString().split("::ffff:")[1]+currentClient->peerPort();
            currentClient->destroyed();
            m_tcpClient.remove(ip);
        }
    });

    m_tcpClient.insert(ip,currentClient);

    // 如果联系人不存在则创建新的联系人信息和对话信息
    bool exist = m_profile->m_contact.contains(ip);
    if(!exist){
        m_profile->m_contact.append(ip);
        profile newContact;
        newContact.ip = ip;
        m_profile->m_contactProfile.append(newContact);
        m_profile->m_chatList.insert(ip, Message());
        emit addContact(newContact);
    }
    // 打招呼
    QString data = "Hello, you can chat to "+m_profile->m_ip+" now!";
    currentClient->write(data.toUtf8());
}
