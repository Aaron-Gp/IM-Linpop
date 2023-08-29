#ifndef LISTBAR_H
#define LISTBAR_H
#define MYLOG qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <profilemanager.h>

class QVBoxLayout;
class QStackedLayout;
class QListWidget;
class QLineEdit;
class QPushButton;
class QToolButton;
class QLabel;

class ListBar : public QFrame
{
    Q_OBJECT
public:
    explicit ListBar(QWidget *parent = nullptr);
    void addContact(profile pf);

private:
    void setupUi();
    void setupTopBar();
    void setupStackBar();
    void makeFakeData();

signals:

public:
    QStackedLayout *stackLLayout;
    QListWidget *messageWidget;
    QJsonArray *fakeListData;


    // 启动服务
    QLineEdit *m_serverPortEditor;
    QPushButton *portSubmitBtn;
    QLabel *m_localIpLabel;

    // 添加联系人
    QLineEdit *searchBar;
    QToolButton *addContactBtn;

private:
    QFrame *m_topBar;
    QVBoxLayout *m_layout;
    QString m_avatarPath = ":/icons/avatar";
    ProfileManager *m_profile;

};

#endif // LISTBAR_H
