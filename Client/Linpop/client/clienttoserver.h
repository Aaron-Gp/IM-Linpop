#ifndef CLIENTTOSERVER_H
#define CLIENTTOSERVER_H

#include <QObject>
#include <QWidget>
#include "global.h"
#include <QTcpSocket>
#include "profilemanager.h"
#include "clientdatabase.h"
#include "msganalyzer.h"

class ClientToServer: public QWidget
{
    Q_OBJECT
public:
    explicit ClientToServer(QWidget *parent = nullptr);
    //    void newConnection(QString ip);
    //    void sendData(QString ip, QString msg);
    void sendToServer(QString msg);
    ~ClientToServer();
    ProfileManager *m_profile;
    QTcpSocket* m_tcpClient;
    QTcpSocket* m_server;
    ClientDataBase* m_db;
    MsgAnalyzer* m_analyzer;
signals:
    void appendMsg(QString ip, message msg);
};

#endif // CLIENTTOSERVER_H
