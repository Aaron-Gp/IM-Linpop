#ifndef CLIENTDATABASE_H
#define CLIENTDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "global.h"
#include <QWidget>
#include <QJsonObject>
#include "profilemanager.h"


class ClientDataBase: public QWidget
{
    Q_OBJECT

public:
    explicit ClientDataBase(QWidget *parent = nullptr);
    bool connectDataBase(QString dbPath);//连接数据库
    QMap<QString,Message> getMessage(QList<QJsonObject>);
    void addProfile(profile new_connect);
    int m_id;

    QMap<QString, Message> getMessage();
    QMap<QString, profile> getProfile();
    bool selectHistoryByData(QString id, QString dataPart, QList<QJsonObject> &jsonMessageList);//客户端查询消息记录
public slots:
    void addMessage(QJsonObject jsonMessage);//客户端存入消息记录
signals:
    void finish();

private:
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // CLIENTDATABASE_H
