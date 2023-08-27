#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QtMaterialAvatar;
class QtMaterialBadge;
class QFrame;

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

private:
    Ui::MainWindow *ui;
    QtMaterialAvatar      *const m_avatar;
    QtMaterialBadge       *const m_badge;
    QJsonArray *fakeListData;
};
#endif // MAINWINDOW_H
