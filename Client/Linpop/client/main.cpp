#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QObject>
#include <profilemanager.h>
#include <stdexcept>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProfileManager *pm = ProfileManager::getInstance();
    pm->m_clientToServer = new QTcpSocket;
    pm->m_clientToServer->connectToHost("192.168.163.130",6666);
    bool ok = pm->m_clientToServer->waitForConnected(1000);
    if(!ok){
        throw std::runtime_error("cannot connect to server!");
    }
    login l;
    MainWindow w;
    l.show();
    QObject::connect(&l, &login::loginSucceed, &w, &MainWindow::rcvLogin);
    QObject::connect(&w, &MainWindow::closeLoginWindow, &l, &login::loginWindowClose);
    return a.exec();
}
