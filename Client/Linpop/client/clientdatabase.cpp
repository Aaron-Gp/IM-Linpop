#include "clientdatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QJsonObject>
#include "global.h"

ClientDataBase::ClientDataBase()
{
}

bool ClientDataBase::connectDataBase(){
    // 创建一个QSqlDatabase对象，用于连接数据库
    db = QSqlDatabase::addDatabase("QSQLITE","data");
    // 指定数据库路径，如果路径不存在，将会创建一个新数据库
    db.setDatabaseName(DATABASE+QString::number(m_id)+DATABASENAME);
    // 尝试建立到数据库的连接
    if (!db.open()) {
        qDebug() << "Failed to connect to database:" << db.lastError();
        return false;
    }
    query=QSqlQuery(db);
    /*删除表
    if(!query.exec("DROP TABLE history;")){
        qDebug() << "Failed to drop table history";
    }*/
    // 创建一个表
    if (!query.exec("CREATE TABLE history (id TEXT, timestamp INTEGER PRIMARY KEY, type TEXT, data TEXT, isSender INTEGER)")) {
        qDebug() << "Failed to create table history:" << query.lastError();
    }

    return true;
}

void ClientDataBase::addMessage(QJsonObject jsonMessage) {
    QString id = QString::number(jsonMessage["id"].toInt());
    int timestamp = jsonMessage["timestamp"].toInt();
    QString type = jsonMessage["type"].toString();
    QString data = jsonMessage["data"].toString();
    bool isSender = jsonMessage["active"].toBool();
    qDebug()<<id<<timestamp<<type<<data<<isSender<<endl;
    query.prepare("INSERT INTO history VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(timestamp);
    query.addBindValue(type);
    query.addBindValue(data);
    query.addBindValue(isSender);

    if (!query.exec())
        qDebug() << "Failed to add history: " << query.lastError();
    qDebug() << "Succeeded in adding history!";
}

bool ClientDataBase::getMessage(QString id, QList<QJsonObject> &jsonMessageList){
    query.prepare("SELECT * FROM history WHERE id = ? ORDER BY timestamp");
    QVariantList a;
    a << id;
    query.addBindValue(a);
    if(!query.execBatch()){
        qDebug() << "Failed to select the history: " << query.lastError();
        return false;
    }
    while(query.next()){
        QJsonObject jsonMessage;
        jsonMessage["id"] = query.value("id").toString();
        jsonMessage["timestamp"] = query.value("timestamp").toString();
        jsonMessage["type"] = query.value("type").toString();
        jsonMessage["data"] = query.value("data").toString();
        jsonMessage["isSender"] = query.value("isSender").toInt();
        jsonMessageList.append(jsonMessage);
    }
    qDebug() << "Succeed to get the history!";//isSender为true的时候说明发送者是该用户
    return true;
}

bool ClientDataBase::selectHistoryByData(QString id, QString dataPart, QList<QJsonObject> &jsonMessageList){
    query.prepare("SELECT * FROM history WHERE id = ? AND data LIKE ? ORDER BY timestamp");
    QVariantList a,b;
    a << id;
    b << "%" + dataPart + "%";
    query.addBindValue(a);
    query.addBindValue(b);
    if(!query.execBatch()){
        qDebug() << "Failed to select the history by data: " << query.lastError();
        return false;
    }
    while(query.next()){
        QJsonObject jsonMessage;
        jsonMessage["id"] = query.value("id").toString();
        jsonMessage["timestamp"] = query.value("timestamp").toString();
        jsonMessage["type"] = query.value("type").toString();
        jsonMessage["data"] = query.value("data").toString();
        jsonMessage["isSender"] = query.value("isSender").toInt();
        jsonMessageList.append(jsonMessage);
    }
    qDebug() << "Succeed to select the history by data!";
    return true;
}


/*
    //查看消息记录
    QList<QJsonObject> jsonMessageList;
    getMessage("10086",jsonMessageList);
    foreach (QJsonObject jsonMessage, jsonMessageList) {
        qDebug() << "id: " << jsonMessage["id"];
        qDebug() << "timestamp: " << jsonMessage["timestamp"];
        qDebug() << "type: " << jsonMessage["type"];
        qDebug() << "data: " << jsonMessage["data"];
        qDebug() << "isSender: " << jsonMessage["isSender"];
    }
*/
