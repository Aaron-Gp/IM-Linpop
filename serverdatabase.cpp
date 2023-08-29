#include "serverdatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

ServerDataBase::ServerDataBase()
{

}

bool ServerDataBase::connectDataBase(){
    // 创建一个QSqlDatabase对象，用于连接数据库
    db = QSqlDatabase::addDatabase("QSQLITE","sdb");

    // 指定数据库路径，如果路径不存在，将会创建一个新数据库
    db.setHostName("host");
    db.setDatabaseName("../ServerDataBase/ServerDataBase.db");
    db.setUserName("administrator");
    db.setPassword("123456");

    //查找数据库在本地的位置
    /*QString databasePath = QCoreApplication::applicationDirPath() + "/:memory:.db";
    QString newDatabasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/:memory:.db";
    qDebug() << databasePath;
    qDebug() << newDatabasePath;
    //QFile::copy(databasePath, newDatabasePath);
    */

    // 尝试建立到数据库的连接
    if (!db.open()) {
        qDebug() << "Failed to connect to database:" << db.lastError();
        return false;
    }

    query=QSqlQuery(db);

    /*
    //删除表
    if(!query.exec("DROP TABLE message;")){
        qDebug() << "Failed to drop table user: " << query.lastError();
    }
    */

    // 创建一个表（如果表不存在）
    if (!query.exec("CREATE TABLE user (id TEXT PRIMARY KEY, name TEXT, password TEXT, online INTEGER DEFAULT 0, ip TEXT, avatar BLOB, department TEXT)")) {
        qDebug() << "Failed to create table user:" << query.lastError();
    }
    if (!query.exec("CREATE TABLE message (idSender TEXT, idReceiver TEXT, timestamp TEXT, type TEXT, data TEXT, PRIMARY KEY(idReceiver, timestamp))")) {
        qDebug() << "Failed to create table message:" << query.lastError();
    }

    // 创建用户的视图，包括头像avatar,工号id,ip,姓名name,部门department,在线情况online
    if (!query.exec("CREATE View user_view AS SELECT avatar, id, ip, name, department, online from user")) {
        qDebug() << "Failed to create view user_view:" << query.lastError();
    }


    return true;
}

//user相关代码
bool ServerDataBase::isUserAccountCorrect(QString id, QString password){
    query.prepare("SELECT * FROM user WHERE id = ? ");
    QVariantList a;//a用来存储id
    a << id;
    query.addBindValue(a);
    if(!query.execBatch()){
        qDebug() << "Failed to select id : " << query.lastError();
        return false;//查找功能出错
    }

    if(query.next()){
        QString passwordToId = query.value("password").toString();
        if(encryptUserPassword(password)!= passwordToId){
            qDebug() << "The password is wrong."; //密码错误
            return false;
        }
    }else{
        qDebug() << "Failed to find the id. "; //数据库中没有这个id存在
        return false;
    }

    qDebug() << "The account is correct!";
    query.prepare("UPDATE user SET online = 1 WHERE id = ?");//账号登陆状态为已登陆
    query.addBindValue(a);
    if(!query.execBatch()){
        qDebug() << "Failed to set online true : " << query.lastError();
        return false;//查找功能出错
    }
    return true;
}

bool ServerDataBase::setUserAccountOffline(QString id){
    query.prepare("UPDATE user SET online = 0 WHERE id = ?");
    QVariantList a;
    a << id;
    query.addBindValue(a);
    if(!query.execBatch()){
        qDebug() << "Failed to set online false: " << query.lastError();
        return false;//设置失败
    }
    return true;//成功设置离线
}

bool ServerDataBase::addUserAccount(QString id, QString name, QString password){
    query.prepare("SELECT * FROM user WHERE id = ?");
    QVariantList a,b,c;
    a << id;
    query.addBindValue(a);
    if(!query.execBatch()){
        qDebug() << "Failed to tell if the id exists: " << query.lastError();
        return false;
    }
    if(query.next()){//查找这个id是不是用过了
        qDebug() << "The id exists.";//id用过了
        return false;
    }else{
        query.prepare("INSERT INTO user (id, name, password) VALUES (?, ?, ?)");
        b << name;
        c << encryptUserPassword(password);
        query.addBindValue(a);
        query.addBindValue(b);
        query.addBindValue(c);
        if(!query.execBatch()){
            qDebug() << "Failed to add account: " << query.lastError();
            return false;//加入失败
        }
        qDebug() << "Succeed to add account!";//成功添加账户
        return true;
    }
}

bool ServerDataBase::alterUserPassword(QString id, QString passwordOld, QString passwordNew){
    query.prepare("SELECT * FROM user WHERE id = ?");
    QVariantList a;
    a << id;
    query.addBindValue(a);
    if(!query.execBatch()){
        qDebug() << "Failed to find the id: " << query.lastError();
        return false;//查找操作失败
    }
    if(!query.next()){
        qDebug() << "The id doesn't exist.";
        return false;//账户不存在
    }else{
        QString passwordToId = query.value("password").toString();
        if(passwordToId!=encryptUserPassword(passwordOld)){
            qDebug() << "The old password isn't correct.";
            return false;//原密码不正确
        }else{
            query.prepare("UPDATE user SET password = ? WHERE id = ?");
            QVariantList a,b;
            a << encryptUserPassword(passwordNew);
            b << id;
            query.addBindValue(a);
            query.addBindValue(b);
            if(!query.execBatch()){
                qDebug() << "Failed to alter password: " << query.lastError();
                return false;//修改失败
            }
            qDebug() << "Succeed to alter password!";//成功修改密码
            return true;
        }
    }
}

bool ServerDataBase::alterUserName(QString id, QString name){
    query.prepare("SELECT * FROM user WHERE id = ?");
    QVariantList a;
    a << id;
    query.addBindValue(a);
    if(!query.execBatch()){
        qDebug() << "Failed to find the id: " << query.lastError();
        return false;//查找操作失败
    }
    if(!query.next()){
        qDebug() << "The id doesn't exist.";
        return false;//账户不存在
    }else{
        query.prepare("UPDATE user SET name = ? WHERE id = ?");
        QVariantList a,b;
        a << name;
        b << id;
        query.addBindValue(a);
        query.addBindValue(b);
        if(!query.execBatch()){
            qDebug() << "Failed to alter name: " << query.lastError();
            return false;//修改失败
        }
        qDebug() << "Succeed to alter name!";//成功修改用户名
        return true;
        }
}

bool ServerDataBase::deleteUserAccount(QString id){
    query.prepare("SELECT * FROM user WHERE id = ?");
    QVariantList a;
    a << id;
    query.addBindValue(a);
    if(!query.execBatch()){
        qDebug() << "Failed to select id: " << query.lastError();
        return false;//搜索失败
    }
    if(!query.next()){
        qDebug() << "The id doesn't exist.";
        return false;//id不存在
    }else{
        query.prepare("DELETE FROM user WHERE id = ?");
        query.addBindValue(a);
        if(!query.execBatch()){
            qDebug() << "Failed to delete account: " << query.lastError();
            return false;//删除失败
        }
        qDebug() << "Succeed to delete account!";//成功删除账号
        return true;
    }
}

bool ServerDataBase::isUserOnline(QString id){
    query.prepare("SELECT online FROM user WHERE id = ?");
    QVariantList a;
    a << id;
    query.addBindValue(a);
    if(!query.execBatch()){
        qDebug() << "Failed to select id: " << query.lastError();
        return false;//查询失败
    }
    if(query.next()){
        int online=query.value("online").toInt();
        return online;
    }
    qDebug() << "Failed to find the id.";
    return false;//id不存在
}

QString ServerDataBase::encryptUserPassword(QString password){
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(password.toUtf8());
    QString hashedPassword = hash.result().toHex();
    return hashedPassword;
}

bool ServerDataBase::getUserView(QString id, QJsonObject &jsonUserView){
    query.prepare("SELECT * FROM user_view WHERE id = ?");
    QVariantList a;
    a << id;
    query.addBindValue(a);
    if(!query.execBatch()){
        qDebug() << "Failed to select from user_view: " << query.lastError();
        return false;
    }
    if(!query.next()){
        qDebug() << "The id doesn't exist." << query.lastError();
    }
    jsonUserView["avatar"] = query.value("avatar").toString();
    jsonUserView["id"] = id;
    jsonUserView["ip"] = query.value("ip").toString();
    jsonUserView["name"] = query.value("name").toString();
    jsonUserView["department"] = query.value("department").toString();
    jsonUserView["online"] = query.value("department").toInt();
    qDebug() << "Succeed to select from user_view!";
    return true;
}

//message相关代码
bool ServerDataBase::addMessage(QJsonObject jsonMessage){
    QVariantList idSender;idSender << jsonMessage["idSender"].toVariant();
    QVariantList idReceiver;idReceiver << jsonMessage["idReceiver"].toVariant();
    QVariantList timestamp;timestamp << jsonMessage["timestamp"].toVariant();
    QVariantList type;type << jsonMessage["type"].toVariant();
    QVariantList data;data << jsonMessage["data"].toVariant();
    query.prepare("INSERT INTO message VALUES (?,?,?,?,?)");
    query.addBindValue(idSender);
    query.addBindValue(idReceiver);
    query.addBindValue(timestamp);
    query.addBindValue(type);
    query.addBindValue(data);
    if(!query.execBatch()){
        qDebug() << "Failed to add message: " << query.lastError();
        return false;
    }
    qDebug() << "Succeed to add message!";
    return true;
}

bool ServerDataBase::getMessage(QString idReceiver, QList<QJsonObject> &jsonMessageList){
    query.prepare("SELECT * FROM message WHERE idReceiver = ? ORDER BY timestamp DESC");
    QVariantList a;
    a << idReceiver;
    query.addBindValue(a);
    if(!query.execBatch()){
        qDebug() << "Failed to select the message: " << query.lastError();
        return false;
    }
    while(query.next()){
        QJsonObject jsonMessage;
        jsonMessage["idSender"] = query.value("idSender").toString();
        jsonMessage["idReceiver"] = query.value("idReceiver").toString();
        jsonMessage["timestamp"] = query.value("timestamp").toString();
        jsonMessage["type"] = query.value("type").toString();
        jsonMessage["data"] = query.value("data").toString();
        jsonMessageList.append(jsonMessage);
    }
    qDebug() << "Succeed to get the message!";
    return true;
}


void ServerDataBase::printTableUser(){
    if(!query.exec("SELECT * FROM user")){
        qDebug() << "Failed to select the table: " << query.lastError();
        return;
    }else{
        while (query.next()) {
            QString id = query.value("id").toString();
            QString name = query.value("name").toString();
            QString password = query.value("password").toString();
            bool online = query.value(3).toInt();
            qDebug() << "id: " << id << " name: " << name << " password: " << password << " online: " << online;
        }
    }
}





/*
    query.prepare("UPDATE user SET online = 0 WHERE id = ?");
    QVariantList a;
    a << id;
    query.addBindValue(a);
    if(!query.execBatch()){
        qDebug() << "Failed to set online false: " << query.lastError();
        return false;
    }
*/
/*
    //查看消息记录
    QList<QJsonObject> jsonMessageList;
    getMessage("12306",jsonMessageList);
    foreach (QJsonObject jsonMessage, jsonMessageList) {
        qDebug() << "idSender: " << jsonMessage["idSender"];
        qDebug() << "idReceiver: " << jsonMessage["idReceiver"];
        qDebug() << "timestamp: " << jsonMessage["timestamp"];
        qDebug() << "type: " << jsonMessage["type"];
        qDebug() << "data: " << jsonMessage["data"];
    }
*/
