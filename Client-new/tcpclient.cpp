#include "tcpclient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

TcpClient::TcpClient(QWidget *parent){
    m_client = new QTcpSocket(this);
    //连接服务器
    m_client->connectToHost(QHostAddress("127.0.0.1"), 6666);
    qDebug() << m_client->waitForConnected();
    //通过信号接收服务器数据
    connect(m_client, &QTcpSocket::readyRead,
            this, &TcpClient::slotReadyRead);
    //发送按钮
}

TcpClient::~TcpClient()
{
}

void TcpClient::slotReadyRead()
{
    //接收数据
    QByteArray array = m_client->readAll();

    QMessageBox::information(this, "Server Message", array);;
}

void TcpClient::slotSendMsg(QString text)
{
    QJsonObject jsonObj;
    // 向 JSON 对象中添加键值对
    jsonObj["data"] = text;
    jsonObj["function"]="information";
    jsonObj["sender"]=1;
    jsonObj["receiver"]=2;
    jsonObj["type"]="text";

    QJsonDocument jsonDoc(jsonObj);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);
    m_client->write("<?BEGIN?>");
    m_client->write(jsonString.toUtf8());
    m_client->write("<?END?>");
}

