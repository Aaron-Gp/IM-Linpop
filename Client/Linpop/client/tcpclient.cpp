#include "tcpclient.h"
#include <QMessageBox>

TcpClient::TcpClient(QWidget *parent) : QWidget(parent)
{
    //初始化TCP客户端
    tcpClient = new QTcpSocket();   //实例化tcpClient
    tcpClient->abort();                 //取消原有连接
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), \
            this, SLOT(ReadError(QAbstractSocket::SocketError)));
    profile = ProfileManager::getInstance();
}

TcpClient::~TcpClient()
{
    tcpClient->abort();
}

void TcpClient::ReadData()
{
    QByteArray buffer = tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        qDebug()<<QString::fromUtf8(buffer);
    }
}

void TcpClient::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
    QMessageBox msgBox;
    msgBox.setText(tr("failed to connect server because %1").arg(tcpClient->errorString()));
    msgBox.exec();
}


