#ifndef LISTBAR_H
#define LISTBAR_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

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
    QLineEdit *m_serverPortEditor;
    QPushButton *portSubmitBtn;
    QLabel *m_localIpLabel;
    QLineEdit *searchBar;
    QToolButton *addContactBtn;

private:
    QFrame *m_topBar;
    QVBoxLayout *m_layout;
    QString m_avatarPath = ":/icons/avatar";

};

#endif // LISTBAR_H
