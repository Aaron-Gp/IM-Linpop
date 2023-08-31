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
#include "clientdatabase.h"
#include "profilemanager.h"

class MsgAnalyzer:public QObject{
    Q_OBJECT
public:
    static MsgAnalyzer* getInstance();
    ClientDataBase* db;
    void sendError(QTcpSocket *socket, QString error);
    void anaylze(QTcpSocket *socket, QString message);
    void sendMessage(QTcpSocket *socket, QString function, message* msg);
    void receiveMessage(QTcpSocket *socket, QByteArray msg, bool isServer);

    ProfileManager *m_profile;

signals:
    void successLogin();
    void storeIntoDatabase(QJsonObject information);
public slots:
    void readMessage(QTcpSocket* socket);

private:
    MsgAnalyzer();
    static MsgAnalyzer *self;
};
#endif // MSGANALYZE_H
