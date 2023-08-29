#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login l;
    MainWindow w;
    l.show();
    QObject::connect(&l, &login::loginSucceed, &w, &MainWindow::rcvLogin);
    QObject::connect(&w, &MainWindow::closeLoginWindow, &l, &login::loginWindowClose);
    return a.exec();
}
