#ifndef MAINBAR_H
#define MAINBAR_H

#include <QWidget>
#include <QFrame>
#include <qnchatmessage.h>

class QVBoxLayout;
class QTextEdit;
class QListWidget;
class QListWidgetItem;
class QSplitter;

class MainBar : public QFrame
{
    Q_OBJECT
public:
    explicit MainBar(QWidget *parent = nullptr);
    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
    void dealMessageTime(QString curMsgTime);
    void changeBar(QString name, QString ip);
    void addMessage(QJsonObject msg);
    void addMessages(QJsonArray msgs);
    void clearBroswer();


private:
    void setupUi();
    void setupTopBar();
    void setupMainBar();

signals:

private:
    QFrame *m_topBar;
    QVBoxLayout *m_layout;
    QString m_avatarPath = ":/icons/avatar";
    QTextEdit *chatEditor;
    QListWidget *chatBroswer;
    QString m_title="";
    QSplitter *splitter;
};

#endif // MAINBAR_H
