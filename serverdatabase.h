#ifndef SERVERDATABASE_H
#define SERVERDATABASE_H


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonObject>
#include <QCryptographicHash>

class ServerDataBase
{
public:
    ServerDataBase();
    bool connectDataBase();//连接数据库

    //user相关代码
    bool isUserAccountCorrect(QString id, QString password);//判断用户登陆信息是否正确(函数出错时返回false，但是程序会继续运行)
    bool setUserAccountOffline(QString id);//(客户端关闭时传入信号让服务端)设置用户离线
    bool addUserAccount(QString id, QString name, QString password);//向数据库添加用户
    bool alterUserPassword(QString id, QString passwordOld, QString passwordNew);//修改密码（先判断旧密码是否正确，再进行密码修改)
    bool alterUserName(QString id, QString name);//修改用户名
    bool deleteUserAccount(QString id);//直接删除对应账号
    bool isUserOnline(QString id);//用户是否在线(函数出错时返回false，但是程序会继续运行)
    QString encryptUserPassword(QString password);//用系统的库QCryptographicHash进行加密，这个加密是不可逆的
    bool getUserView(QString id, QJsonObject &jsonUserView);//使用用户视图，用户在查询好友信息时可能用到，允许查看头像avatar,工号id,ip,姓名name,部门department,在线情况online

    //message相关代码
    bool addMessage(QJsonObject jsonMessage);//服务端接收信息
    bool getMessage(QString idReceiver, QList<QJsonObject> &jsonMessageList);//服务端将接受者累计的消息根据时间从头到尾全部放入列表中


    void printTableUser();//用来调试的时候输出表格全部内容，防止管理没有装sqlite3;

private:
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // SERVERDATABASE_H
