#include "clienttoserver.h"

ClientToServer::ClientToServer(QWidget *parent):QWidget(parent)
{
    m_profile = ProfileManager::getInstance();
    m_db=new ClientDataBase;
    m_analyzer = new MsgAnalyzer(m_db);
    //用于和服务器连接
    m_server = new QTcpSocket();   //实例化tcpClient
    m_server->connectToHost(SERVER, PORT);
    bool ok = m_server->waitForConnected(1000);
    if (!ok)
        MYLOG<<"fail to connect server";
    else
        MYLOG<<"success to connect server";
    connect(m_server, &QTcpSocket::readyRead, m_analyzer,[&](){
        m_analyzer->readMessage(m_server);
    });
    connect(m_server, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), [&](){
        m_server->disconnectFromHost();
        QMessageBox msgBox;
        msgBox.setText(tr("disconnect with server"));
        msgBox.exec();
    });
}

void ClientToServer::sendToServer(QString msg)
{
    m_server->write(msg.toUtf8());
}

ClientToServer::~ClientToServer()
{
    m_server->abort();
}
