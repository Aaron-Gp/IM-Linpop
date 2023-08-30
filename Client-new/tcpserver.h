#ifndef TCPSERVER_H
#define TCPSERVER_H



#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

class TCPServer : public QWidget
{
    Q_OBJECT
public:
    explicit TCPServer(QWidget *parent = 0);
public slots:
    void slotNewConnection();
    void slotReadyRead();
private:
    // 负责监听的套接字
    QTcpServer* m_server;
    // 负责通信的套接字
    QTcpSocket* m_client;
};


#endif // TCPSERVER_H
