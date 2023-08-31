#include "tcpserver.h"
#include <QDateTime>

TcpServer::TcpServer(QWidget *parent) : QWidget(parent)
{
    tcpServer = new QTcpServer();
    m_profile = ProfileManager::getInstance();
    m_analyzer = MsgAnalyzer::getInstance();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));
}

TcpServer::~TcpServer()
{
    closeAll();
}

void TcpServer::closeAll()
{
    QMapIterator<QString, QTcpSocket*>it(m_tcpClient);
    while (it.hasNext())
    {
        it.next();
        QTcpSocket *tcpClient = it.value();
        bool ok = tcpClient->waitForDisconnected(1000);
        if(!ok)
        {
            MYLOG<<"closed error";
            tcpClient->abort();
        }
        m_tcpClient.remove(it.key());
    }
    m_tcpClient.clear();
    tcpServer->close();     //不再监听端口
}

void TcpServer::NewConnectionSlot()
{
    currentClient = tcpServer->nextPendingConnection();
    QString ip = currentClient->peerAddress().toString().split("::ffff:")[1]+":"+QString::number(currentClient->peerPort());
    MYLOG<<"new connection from "<<ip;

    connect(currentClient, &QTcpSocket::readyRead, [=](){
        QByteArray buffer = currentClient->readAll();
        if(!buffer.isEmpty()){
            m_analyzer->receiveMessage(currentClient, buffer, true);
        }
    });

    connect(currentClient, &QTcpSocket::disconnected, [=](){
        MYLOG<<"client diconnected";
        if(currentClient->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在tcpClient列表中的客户端信息
            QString ip = currentClient->peerAddress().toString().split("::ffff:")[1]+":"+QString::number(currentClient->peerPort());
            currentClient->destroyed();
            MYLOG<<"destroy "<<ip;
            m_tcpClient.remove(ip);
        }
    });

    m_tcpClient.insert(ip,currentClient);
    // 打招呼
    m_analyzer->sendMessage(currentClient, "init", nullptr);
}
