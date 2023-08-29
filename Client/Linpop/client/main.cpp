#include "mainwindow.h"
#include <QApplication>
#include "login.h"
#include "QObject"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login loginPage;
    loginPage.show();
    MainWindow w;
    QObject::connect(&loginPage,&Login::tryLogin,&w,&MainWindow::tryLogin);
    QObject::connect(&w,&MainWindow::successLogin,&loginPage,&Login::hide);
    return a.exec();
}
