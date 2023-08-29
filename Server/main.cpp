#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QVBoxLayout>
#include <QThreadPool>
#include "mainwindow.h"
#include <QApplication>
#include "msg.h"
#include "filemanager.h"

int main(int argc, char *argv[]){
    qRegisterMetaType<Msg>("Msg");
    QApplication a(argc, argv);
    MainWindow mainWindow;// 创建 QTableView 控件
    // 获取全局线程池实例
    QThreadPool* threadPool = QThreadPool::globalInstance();
    // 设置线程池的最大线程数
    threadPool->setMaxThreadCount(10);
    // 设置线程池的自动删除任务
    threadPool->setExpiryTimeout(1000); // 30 seconds
    mainWindow.show();
    return a.exec();
}
