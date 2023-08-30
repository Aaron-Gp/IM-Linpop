#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <qnchatmessage.h>
#include <sidebar.h>
#include <listbar.h>
#include <mainbar.h>
#include <profilemanager.h>
#include "tcpserver.h"
#include "tcpclient.h"
#include "global.h"
#include <msganalyzer.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QtMaterialAvatar;
class QtMaterialBadge;
class QFrame;
class QListWidget;
class QListWidgetItem;
class QTextEdit;
class QGridLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setUpUi();
    QFrame* leftUi();
    QFrame* middleUi();
    QFrame* rightUi();
    void makeFakeData();
    QGridLayout *mainLayout;
    MsgAnalyzer *m_analyzer;

signals:
    void switched(int index);
    void closeLoginWindow();
public slots:
    void rcvLogin();

private:
    Ui::MainWindow *ui;
    SideBar *m_sideBar;
    ListBar *m_listBar;
    MainBar *m_mainBar;
    QJsonArray *fakeListData;
    QTextEdit *m_chatEditor;
    QListWidget *m_chatBroswer;
    ProfileManager *m_profile;
    TcpServer *m_server;
    TcpClient *m_client;

};
#endif // MAINWINDOW_H
