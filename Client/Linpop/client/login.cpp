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

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    connect(ui->loginButon_2,&QPushButton::clicked,this,[this](){
        QString id=ui->accountlineEdit_2->text();
        QString password=ui->passwordlineEdit_2->text();
        emit tryLogin(id,password);
    });
}

Login::~Login()
{
    delete ui;
}

