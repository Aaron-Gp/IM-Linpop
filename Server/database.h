#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlQuery>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTableView>
#include <QSqlQueryModel>
#include <qheaderview.h>
#include <QSortFilterProxyModel>
#include <QComboBox>
#include <QRandomGenerator>
#include <QJsonObject>

class DataBase:QObject{
    Q_OBJECT
public:
    DataBase(QString path,QObject* parent=0);
    ~DataBase();
    void exec(QString sql);
    void close();
    void ShowInTable(QTableView* view,QString sql);
    void ShowInComboBox(QComboBox *cb, QString sql);
    static int count;
public:
    QSqlQuery query;
    QSqlDatabase database;
    int isUserAccountCorrect(int id, QString password);
    void setUserAccountOffline(int id);
    static QString generateRandomPassword(int length);
    QJsonObject addUserAccount(int id, QString name, int department);
    QString alterUserPassword(int id, QString passwordOld, QString passwordNew);
    QString alterUserName(int id, QString name);
    void deleteUserAccount(int id);
    bool isUserOnline(int id);
    void addMessage(QJsonObject jsonMessage);
    void getMessage(int idReceiver, QList<QJsonObject> &jsonMessageList);
    void printTableUser();
    void deleteMessage(int id);
    void ShowInTable(QTableView *view, QString id, QString name, QString department);
};

#endif // DATABASE_H
