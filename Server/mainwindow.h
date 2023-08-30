#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include "database.h"
#include <QSignalMapper>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QLineEdit>
#include <QComboBox>
#include <tcpserver.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void slotSwitchToPage(int i);
    void slotDeleteFromDataBase();
    void slotSearch();
    void addNewUser();
private:
    void deleteFromDataBase();
    static void switchToPage(int);
public:
    Ui::MainWindow *ui;
    DataBase* db;
    TcpServer* server;
};
#endif // MAINWINDOW_H
