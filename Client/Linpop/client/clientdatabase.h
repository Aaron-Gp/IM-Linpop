#ifndef CLIENTDATABASE_H
#define CLIENTDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "global.h"
#include <QObject>
#include <QJsonObject>

class ClientDataBase:public QObject
{
    Q_OBJECT
public:
    explicit ClientDataBase();
    bool connectDataBase();//连接数据库
    bool getMessage(QString id, QList<QJsonObject> &jsonMessageList);//客户端输出消息记录
    bool selectHistoryByData(QString id, QString dataPart, QList<QJsonObject> &jsonMessageList);//客户端查询消息记录
    int m_id;
public slots:
    void addMessage(QJsonObject jsonMessage);//客户端存入消息记录
private:
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // CLIENTDATABASE_H
