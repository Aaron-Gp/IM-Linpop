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

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint|this->windowFlags());


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
        QString username = ui->accountlineEdit->text();
        QString password = ui->passwordlineEdit->text();

        if (username.isEmpty() || password.isEmpty()) {
            qDebug() << "Username and password are required.";
            return;
        }

        // Save password locally
//        if (rememberPassword) {
//            settings->setValue("username", username);
//            settings->setValue("password", password);
//        }

        // Construct the login request
        QUrl loginUrl(""); // Replace with your actual backend URL
        QNetworkRequest request(loginUrl);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        // Prepare POST data
        QString postData = "username=" + username.toUtf8().toPercentEncoding() +
                           "&password=" + password.toUtf8().toPercentEncoding();

        // Send the login request
        QNetworkReply *reply = networkManager->post(request, postData.toUtf8());
        connect(reply, &QNetworkReply::finished, this, [=]() {
            handleLoginResponse(reply);
        });
}

void login::handleLoginResponse(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);

        if (!jsonResponse.isNull() && jsonResponse.isObject()) {
            QJsonObject jsonObject = jsonResponse.object();
            QJsonValue successValue = jsonObject.value("success");

            if (successValue.isBool()) {
                bool loginSuccess = successValue.toBool();

                if (loginSuccess) {
                    // Login successful
                    QMessageBox::information(this, "Login Successful", "Welcome!");
                    emit loginSucceed();
                } else {
                    // Login failed
                    QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
                }
            }
        }
    } else {
        // Network error
        QMessageBox::critical(this, "Network Error", "An error occurred while connecting to the server.");
    }
    reply->deleteLater(); // Clean up
}

