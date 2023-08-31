#include "clientdatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QJsonObject>
#include "filemanager.h"
#include "profilemanager.h"

ClientDataBase::ClientDataBase(QWidget *parent):QWidget(parent)
{

}

bool ClientDataBase::connectDataBase(QString dbPath){
    // 创建一个QSqlDatabase对象，用于连接数据库
    db = QSqlDatabase::addDatabase("QSQLITE","cdb");
    // 指定数据库路径，如果路径不存在，将会创建一个新数据库
    db.setDatabaseName(dbPath);

    // 尝试建立到数据库的连接
    if (!db.open()) {
        qDebug() << "Failed to connect to database:" << db.lastError();
        return false;
    }

    query=QSqlQuery(db);

    //删除表
//    if(!query.exec("DROP TABLE history;")){
//        qDebug() << "Failed to drop table history";
//    }

    // 创建一个表
    if (!query.exec("CREATE TABLE history (uid integer primary key autoincrement, id TEXT, timestamp INTEGER, type TEXT, data TEXT, isSender INTEGER)")) {
        qDebug() << "Failed to create table history:" << query.lastError();
    }

    if (!query.exec("CREATE TABLE contact (id TEXT PRIMARY KEY, name varchar(256), avatar blob,ip varchar(128))")) {
        qDebug() << "Failed to create table contact:" << query.lastError();
    }

    return true;
}

void ClientDataBase::addMessage(QJsonObject jsonMessage){
    QString id = jsonMessage["id"].toString();
    int timestamp = jsonMessage["timestamp"].toInt();
    QString type = jsonMessage["type"].toString();
    QString data = jsonMessage["data"].toString();
    int isSender = jsonMessage["isSender"].toInt();
    if(type=="file")
            data=FileManager::ToFile(jsonMessage["fileName"].toString(),id,isSender,data);
    qDebug()<<id<<timestamp<<type<<data<<isSender<<endl;
    query.prepare("INSERT INTO history VALUES (NULL, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(timestamp);
    query.addBindValue(type);
    query.addBindValue(data);
    query.addBindValue(isSender);
    if (!query.exec())
        qDebug() << "Failed to add history: " << query.lastError();
    qDebug() << "Succeeded in adding history!";
    emit finish();
}

QMap<QString,Message> ClientDataBase::getMessage(){
    if(!query.exec("SELECT * FROM history ORDER BY uid"))
        qDebug() << "Failed to select the history: " << query.lastError();
    QMap<QString,Message> messages;
    Message msgs;
    while(query.next()){
        message msg;
        msg.id=QString::number(query.value("id").toInt());
        msg.time=QString::number(query.value("timestamp").toInt());
        msg.isSender=query.value("isSender").toInt();
        msg.msg=query.value("data").toString();
        //jsonMessage["type"] = query.value("type").toString();
        if (!messages.contains(msg.id))
            messages[msg.id] = QVector<message>(); // 初始化对应的 QVector
        messages[msg.id].append(msg); // 将消息添加到对应的 QVector 中
    }
    return messages;
}

QMap<QString,profile> ClientDataBase::getProfile(){
    if(!query.exec("SELECT * FROM contact"))
        qDebug() << "Failed to select the history: " << query.lastError();
    QMap<QString,profile> profiles;
    while(query.next()){
        profile profile;
        profile.id=QString::number(query.value("id").toInt());
        profile.name=query.value("name").toString();
        profile.avatar=query.value("avatar").toString();
        profile.ip=query.value("ip").toString();
        profiles[profile.id]=profile;
    }
    return profiles;
}

void ClientDataBase::addProfile(profile new_connect)
{
    query.prepare("INSERT INTO contact (id, name, avatar, ip) VALUES (?, ?, ?, ?)");
    query.addBindValue(new_connect.id.toInt()); // 假设 id 是整数类型
    query.addBindValue(new_connect.name);
    query.addBindValue(new_connect.avatar);
    query.addBindValue(new_connect.ip);

    if (!query.exec()) {
        qDebug() << "Failed to add new contact: " << query.lastError();
    } else {
        qDebug() << "New contact added successfully.";
    }
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
