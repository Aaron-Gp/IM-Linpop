#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#define QT_NO_DEBUG_OUTPUT

#include <QMainWindow>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <qnchatmessage.h>
#include <sidebar.h>
#include <listbar.h>
#include <mainbar.h>

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

signals:
    void switched(int index);

private:
    Ui::MainWindow *ui;
    SideBar *m_sideBar;
    ListBar *m_listBar;
    MainBar *m_mainBar;
    QtMaterialAvatar *m_avatar;
    QtMaterialBadge *m_badge;
    QJsonArray *fakeListData;
    QTextEdit *chatEditor;
    QListWidget *chatBroswer;

};
#endif // MAINWINDOW_H
