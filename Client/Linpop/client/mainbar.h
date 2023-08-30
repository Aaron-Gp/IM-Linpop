#ifndef MAINBAR_H
#define MAINBAR_H

#include <QWidget>
#include <QFrame>
#include <qnchatmessage.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QVector>
#include <profilemanager.h>
#include "global.h"
#include "emojibar.h"

class QVBoxLayout;
class QTextEdit;
class QListWidget;
class QListWidgetItem;
class QSplitter;
class QPushButton;
class QToolButton;

class MainBar : public QFrame
{
    Q_OBJECT
public:
    explicit MainBar(QWidget *parent = nullptr);
    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
    void dealMessageTime(QString curMsgTime);
    void changeBar(QString id);
    void addMessage(message msg);
    void addMessages(Message msgs);
    void clearBroswer();

    EmojiBar *bar;
    QPushButton *sendBtn;
    QFrame *historyFrame, *toolCFrame, *chatFrame, *chatCFrame, *sendCFrame;
    QToolButton *emojiBtn, *imageBtn, *fileBtn;

private:
    void setupUi();
    void setupTopBar();
    void setupMainBar();

signals:

public:
    QFrame *m_topBar;
    QVBoxLayout *m_layout;
    QString m_avatarPath = ":/icons/avatar";
    QTextEdit *m_chatEditor;
    QListWidget *m_chatBroswer;
    QString m_title="";
    QSplitter *m_splitter;
    ProfileManager *m_profile;
};

#endif // MAINBAR_H
