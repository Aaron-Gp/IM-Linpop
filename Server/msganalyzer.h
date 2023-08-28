#ifndef MSGANALYZER_H
#define MSGANALYZER_H

#include "msg.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "database.h"
#include "Definition.h"
#include <QObject>
#include <QMetaObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDateTime>
#include <QThread>
#include "client.h"

class TcpServer;

class MsgAnalyzer:public QObject{
    Q_OBJECT
public:
    explicit MsgAnalyzer(Msg msg,QList<Client>* m_clients,QObject* parent=0);
    void anaylze();
    void storeIntoDatabase(QJsonObject information);
    bool tryConnect(QString ip);
    ~MsgAnalyzer();
    void sendError(QTcpSocket *socket, QString error);
    void sendResult(QTcpSocket *socket, QString function, QString error);
    void sendJson(QTcpSocket *socket, QJsonObject json);
signals:
    void finish();
    void send(QTcpSocket* socket,Msg msg);
private:
    Msg msg;
    DataBase db;
    QList<Client>* m_clients;
};
#endif // MSGANALYZE_H
