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
    explicit MsgAnalyzer(ClientDataBase* db);
    ClientDataBase* db;
    void storeIntoDatabase(QJsonObject information);
    void sendError(QTcpSocket *socket, QString error);
    void anaylze(QTcpSocket *socket, QString message);

    ProfileManager *m_profile;

signals:
    void successLogin();
public slots:
    void readMessage(QTcpSocket* socket);
};
#endif // MSGANALYZE_H
