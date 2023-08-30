#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QObject>
#include <profilemanager.h>
#include <stdexcept>
#include "clienttoserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProfileManager *pm = ProfileManager::getInstance();
    pm->m_clientToServer = new ClientToServer;
    login l;
    MainWindow w;
    l.show();
    QObject::connect(&l, &login::tryLogin, [=](QString id, QString password){
        QJsonObject json;
        json["function"] = "login";
        json["sender"]=id.toInt();
        json["password"]=password;
        QJsonDocument jsonDoc(json);
        QString jsonString = "<?BEGIN?>"+jsonDoc.toJson(QJsonDocument::Compact)+"<?END?>";
        pm->m_clientToServer->sendToServer(jsonString);
    });
    QObject::connect(pm->m_clientToServer->m_analyzer, &MsgAnalyzer::successLogin, &w, &MainWindow::rcvLogin);
    QObject::connect(&w, &MainWindow::closeLoginWindow, &l, &login::loginWindowClose);
    return a.exec();
}
