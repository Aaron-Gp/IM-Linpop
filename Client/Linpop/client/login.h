#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMouseEvent>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QSettings>

namespace Ui { class login; }

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private:
    Ui::login *ui;
    QPoint m_point;
    QNetworkAccessManager *networkManager; // Declare networkManager variable
    QSettings *settings;

signals:
    void loginSucceed();

public slots:
    void loginWindowClose();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_pushButton_clicked();
    void on_exitButton_clicked();
    void on_passwordlineEdit_textEdited(const QString &arg1);
    void on_loginButon_clicked();
    void handleLoginResponse(QNetworkReply *reply);
};
#endif // LOGIN_H
