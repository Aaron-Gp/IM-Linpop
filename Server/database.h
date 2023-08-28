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
};

#endif // DATABASE_H
