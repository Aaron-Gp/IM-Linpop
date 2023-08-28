#include "tcpserver.h"

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
    for(int i=0; i<tcpClient.length(); i++)//断开所有连接
    {
        tcpClient[i]->disconnectFromHost();
        bool ok = tcpClient[i]->waitForDisconnected(1000);
        if(!ok)
        {
            // 处理异常
        }
        tcpClient.removeAt(i);  //从保存的客户端列表中取去除
    }
    tcpServer->close();     //不再监听端口
}

void TcpServer::NewConnectionSlot()
{
    currentClient = tcpServer->nextPendingConnection();
    tcpClient.append(currentClient);
//    ui->cbxConnection->addItem(tr("%1:%2").arg(currentClient->peerAddress().toString().split("::ffff:")[1])\
//                                          .arg(currentClient->peerPort()));
    connect(currentClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(currentClient, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
    qDebug()<<"new connection from "<<currentClient->peerAddress().toString().split("::ffff:")[1]<<currentClient->peerPort();

    Dict newContact;
    newContact.insert("ip", currentClient->peerAddress().toString().split("::ffff:")[1]+currentClient->peerPort());
    m_profile->m_contact.append(newContact);


    // 打招呼
    QString data = "Hello, you can chat to "+m_profile->m_ip+" now!";
    currentClient->write(data.toUtf8());
}

void TcpServer::disconnectedSlot()
{
    //由于disconnected信号并未提供SocketDescriptor，所以需要遍历寻找
    for(int i=0; i<tcpClient.length(); i++)
    {
        if(tcpClient[i]->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在combox中的客户端信息
//            ui->cbxConnection->removeItem(ui->cbxConnection->findText(tr("%1:%2")\
//                                  .arg(tcpClient[i]->peerAddress().toString().split("::ffff:")[1])\
//                                  .arg(tcpClient[i]->peerPort())));
            // 删除存储在tcpClient列表中的客户端信息
             tcpClient[i]->destroyed();
             tcpClient.removeAt(i);
        }
    }
}

// 客户端数据可读信号，对应的读数据槽函数
void TcpServer::ReadData()
{
    // 由于readyRead信号并未提供SocketDecriptor，所以需要遍历所有客户端
    for(int i=0; i<tcpClient.length(); i++)
    {
        QByteArray buffer = tcpClient[i]->readAll();
        if(buffer.isEmpty())    continue;

        static QString IP_Port, IP_Port_Pre;
        IP_Port = tr("[%1:%2]:").arg(tcpClient[i]->peerAddress().toString().split("::ffff:")[1])\
                                     .arg(tcpClient[i]->peerPort());

        qDebug()<<IP_Port<<" : "<<QString::fromUtf8(buffer);

        //更新ip_port
        IP_Port_Pre = IP_Port;
    }
}
