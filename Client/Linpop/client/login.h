#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMouseEvent>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui { class login; }

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
signals:
    void tryLogin(QString id,QString password);
private:
    Ui::login *ui;
    QPoint m_point;
};
#endif // LOGIN_H
