#ifndef MSGANALYZER_H
#define MSGANALYZER_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QObject>
#include <QMetaObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDateTime>
#include <QThread>
#include <QMessageBox>
#include <QElapsedTimer>

#include "global.h"
#include "clientdatabase.h"

class TcpServer;

class MsgAnalyzer:public QObject{
    Q_OBJECT
public:
    explicit MsgAnalyzer(ClientDataBase* db);
    ClientDataBase* db;
    void sendError(QTcpSocket *socket, QString error);
    void anaylze(QTcpSocket *socket, QString message);
    void anaylze(QTcpSocket *socket, QString message,bool tag);
signals:
    void storeIntoDatabase(QJsonObject information);
    void successLogin(QString name);
    void returnToFriend(QTcpSocket* socket,bool active);
    void ok(QJsonObject);
public slots:
    void readMessage(QTcpSocket* socket);
};
#endif // MSGANALYZE_H
