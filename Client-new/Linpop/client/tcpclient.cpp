#include "tcpclient.h"
#include <QMessageBox>
#include <QDateTime>
#include <QThread>

TcpClient::TcpClient(QWidget *parent) : QWidget(parent)
{
    m_profile = ProfileManager::getInstance();
    m_analyzer = MsgAnalyzer::getInstance();
}

bool TcpClient::newConnection(QString ip)
{
    //初始化TCP客户端
    QTcpSocket *tcpClient = new QTcpSocket();   //实例化tcpClient

    tcpClient->connectToHost(ip.split(":")[0],ip.split(":")[1].toInt());
    bool ok = tcpClient->waitForConnected(1000);
    if (ok)
    {
        QString ip = tcpClient->peerAddress().toString()+":"+QString::number(tcpClient->peerPort());
        MYLOG<<"connect to "+ip;
        m_tcpClient.insert(ip, tcpClient); // 将新的连接加入连接池中
        /*bool exist = m_profile->m_contact.contains(ip);
        if(!exist){
            MYLOG<<"new contact "+ip;
//            m_profile->m_contact.append(ip);
        }*/
        MYLOG<<"send hello message";
        m_analyzer->sendMessage(tcpClient,"init",nullptr);

    }else{
        MYLOG<<"fail to connect "<<ip;
        return false;
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
            m_analyzer->receiveMessage(tcpClient, buffer, false);
        }
    });

    connect(tcpClient, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), [=](){
        tcpClient->disconnectFromHost();
        QString ip = tcpClient->peerAddress().toString()+":"+QString::number(tcpClient->peerPort());
        m_tcpClient.remove(ip);
        QMessageBox msgBox;
        msgBox.setText(tr("failed to connect server because %1").arg(tcpClient->errorString()));
        msgBox.exec();
    });

    return true;
}

void TcpClient::sendData(QString ip, QString msg)
{

}

TcpClient::~TcpClient()
{
    for(auto iter = m_tcpClient.begin(); iter != m_tcpClient.end(); ++iter)
    {
        qDebug()<< "disconnect " << iter.key();
        iter.value()->abort();
        m_tcpClient.remove(iter.key());
    }
}
