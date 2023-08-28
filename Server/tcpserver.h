#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThreadPool>
#include "msg.h"
#include "client.h"
#include "sockettask.h"
#include "msganalyzer.h"


class TcpServer:public QWidget{
    Q_OBJECT
public:
    TcpServer(QObject* parent=0);
    void SendMsg(Msg msg);
public slots:
    void handleNewMessage(QTcpSocket* socket,Msg msg);
    static void SendMsgTo(QTcpSocket *socket, Msg msg);
    void reconnect(QTcpSocket *socket);
signals:
    void signal_newClient(const QString ip, const int port);
    void signal_newMsg(Msg msg);
    void signal_disConncet(const QString ip,const int port);
private:
    QTcpServer* m_server;
    QList<Client>* m_clients;
private slots:
    void slotNewConnection();//form new connection
    void slotReceiveData();
    void slotDisconnect();
};

#endif // TCPSERVER_H
