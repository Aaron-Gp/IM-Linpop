#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MYLOG qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"
//#define QT_NO_DEBUG_OUTPUT

#include <QMainWindow>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <qnchatmessage.h>
#include <sidebar.h>
#include <listbar.h>
#include <mainbar.h>
#include <profilemanager.h>
#include <tcpserver.h>
#include <tcpclient.h>
#include "login.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QtMaterialAvatar;
class QtMaterialBadge;
class QFrame;
class QListWidget;
class QListWidgetItem;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QFrame* leftUi();
    QFrame* middleUi();
    QFrame* rightUi();
    void makeFakeData();
public slots:
    void tryLogin(QString id,QString password);
    void login(QString name);
    void sendToNewFriend(QTcpSocket* socket,bool active);
signals:
    void successLogin();
    void switched(int index);
private:
    Ui::MainWindow *ui;
    SideBar *m_sideBar;
    ListBar *m_listBar;
    MainBar *m_mainBar;
    QtMaterialAvatar *m_avatar;
    QtMaterialBadge *m_badge;
    QJsonArray *fakeListData;
    QTextEdit *m_chatEditor;
    QListWidget *m_chatBroswer;
    ProfileManager *m_profile;
    TcpServer *m_server;
    TcpClient *m_client;
    int m_id;
    QString m_ip;
    QString m_name;
};

#endif // MAINWINDOW_H
