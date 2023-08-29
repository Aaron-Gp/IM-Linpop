#include "login.h"
#include "ui_login.h"
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QSettings>

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    // Initialize networkManager
    networkManager = new QNetworkAccessManager(this);
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint|this->windowFlags());
}

login::~login()
{
    delete ui;
}

void login::loginWindowClose()
{
    this->close();
}

void login::on_pushButton_clicked()
{
    this->showMinimized();
}

void login::on_exitButton_clicked()
{
    this->close();
}

void login::on_passwordlineEdit_textEdited(const QString &arg1)
{
    ui->passwordlineEdit->setEchoMode(QLineEdit::Password);
}

void login::on_loginButon_clicked()
{
    QString id = ui->accountlineEdit->text();
    QString password = ui->passwordlineEdit->text();

    if (id.isEmpty() || password.isEmpty()) {
        qDebug() << "Username and password are required.";
        return;
    }

    emit tryLogin(id,password);
}

