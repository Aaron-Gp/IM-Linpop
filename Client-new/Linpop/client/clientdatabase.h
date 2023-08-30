#ifndef CLIENTDATABASE_H
#define CLIENTDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "global.h"
#include <QObject>
#include <QJsonObject>
#include "profilemanager.h"

class ClientDataBase:public QObject
{
    Q_OBJECT
public:
    explicit ClientDataBase();
    bool connectDataBase();//连接数据库
    QMap<QString,Message> getMessage(QList<QJsonObject>);
    bool selectHistoryByData(QString id, QString dataPart, QList<QJsonObject> &jsonMessageList);//客户端查询消息记录
    void addProfile(profile new_connect);
    int m_id;
public slots:
    void addMessage(QJsonObject jsonMessage);//客户端存入消息记录
    QMap<QString, Message> getMessage();
    QMap<QString, profile> getProfile();
signals:
    void finish();
private:
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // CLIENTDATABASE_H
