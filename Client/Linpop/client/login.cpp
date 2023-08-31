#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    m_profile = ProfileManager::getInstance();
}

login::~login()
{
    delete ui;
}

void login::loginWindowClose()
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

    m_profile->m_id = id;
    m_profile->m_password = password;

    emit tryLogin(id,password);
}

