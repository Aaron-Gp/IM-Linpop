#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>

class TcpClient : public QWidget{
    Q_OBJECT
public:
    explicit TcpClient(QWidget *parent = 0);
    ~TcpClient();
public slots:
    void slotReadyRead();
    void slotSendMsg(QString text);
private:
    QTcpSocket* m_client;
};

#endif // TCPCLIENT_H
