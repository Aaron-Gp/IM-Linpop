#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <profilemanager.h>

class TcpClient : public QWidget
{
    Q_OBJECT
public:
    explicit TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    QTcpSocket *tcpClient;
    ProfileManager *profile;

private slots:
    //客户端槽函数
    void ReadData();
    void ReadError(QAbstractSocket::SocketError);

//    void on_btnConnect_clicked();
//    void on_btnSend_clicked();
//    void on_pushButton_clicked();

};

#endif // TCPCLIENT_H
