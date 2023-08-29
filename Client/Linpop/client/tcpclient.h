#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QAbstractSocket>
#include <profilemanager.h>
#include "global.h"

class TcpClient : public QWidget
{
    Q_OBJECT
public:
    explicit TcpClient(QWidget *parent = nullptr);
    bool newConnection(QString ip);
    void sendData(QString ip, QString msg);
    ~TcpClient();
    ProfileManager *m_profile;
    QMap<QString, QTcpSocket*> m_tcpClient;

signals:
    void appendMsg(QString ip, message msg);

};

#endif // TCPCLIENT_H
