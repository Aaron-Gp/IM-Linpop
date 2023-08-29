#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMouseEvent>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QSettings>
#include <profilemanager.h>
#include "global.h"

namespace Ui { class login; }

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    ProfileManager *m_profile;

private:
    Ui::login *ui;

signals:
    void loginSucceed();
    void tryLogin(QString id, QString password);

public slots:
    void loginWindowClose();

private slots:
    void on_passwordlineEdit_textEdited(const QString &arg1);
    void on_loginButon_clicked();
};
#endif // LOGIN_H
