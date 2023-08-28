#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QTcpSocket>

class Client {
public:
    int port;
    QString ip;
    QTcpSocket* sock;  // 用于存储客户端的 socket 连接
    Client(int port,QString ip,QTcpSocket* sock);
};

#endif // CLIENT_H
