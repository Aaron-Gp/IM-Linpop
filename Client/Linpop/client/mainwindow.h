#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <qnchatmessage.h>

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
    void  makeFakeData();

    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
    void dealMessageTime(QString curMsgTime);

private slots:
    void on_pushButton_clicked();

signals:
    void switched(int index);

private:
    Ui::MainWindow *ui;
    QtMaterialAvatar      *const m_avatar;
    QtMaterialBadge       *const m_badge;
    QJsonArray *fakeListData;
    QTextEdit *chatEditor;
    QListWidget *chatBroswer;
};
#endif // MAINWINDOW_H
