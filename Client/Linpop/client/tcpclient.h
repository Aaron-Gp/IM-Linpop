#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#define MYLOG qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QDebug>
#include <profilemanager.h>
#include "global.h"
#include "msganalyzer.h"
#include "clientdatabase.h"

class TcpClient : public QWidget
{
    Q_OBJECT
public:
    explicit TcpClient(QWidget *parent = nullptr);
    void newConnection(QString ip);
    void sendData(QString ip, QString msg);
    void sendToServer(QString msg);
    ~TcpClient();
    ProfileManager *m_profile;
    QMap<QString, QTcpSocket*> m_tcpClient;
    QTcpSocket* m_server;
    ClientDataBase* db;
    MsgAnalyzer* analyzer;
signals:
    void appendMsg(QString ip, message msg);
    void AddFriend(QTcpSocket* socket,bool active);
public slots:
    void callForAnaylzer(QTcpSocket* socket,QString string);
};

#endif // TCPCLIENT_H
