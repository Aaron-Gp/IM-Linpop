#include "mainbar.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QSplitter>
#include <QToolButton>
#include <QTextEdit>
#include <QDateTime>
#include <QDebug>

MainBar::MainBar(QWidget *parent) : QFrame(parent)
{
    setFrameStyle(QFrame::Box);
    setLineWidth(0);
    QSizePolicy mFSizePolicy = sizePolicy();
    mFSizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    setSizePolicy(mFSizePolicy);
    m_layout = new QVBoxLayout;
    m_layout->setAlignment(Qt::AlignTop);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    m_profile = ProfileManager::getInstance();
    setLayout(m_layout);
    setupUi();
}

void MainBar::setupUi()
{
    setupTopBar();
    setupMainBar();
}

void MainBar::setupTopBar()
{
    m_topBar = new QFrame;
    m_topBar->setFrameStyle(QFrame::Box);
    m_topBar->setLineWidth(0);
    m_topBar->setFixedHeight(54);
    m_topBar->setStyleSheet(".QFrame{"
                            "border: 1px solid rgb(212,212,212);"
                            "}");
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setAlignment(Qt::AlignLeft);
    QLabel *title =new QLabel(m_title);
    titleLayout->addWidget(title);
    m_topBar->setLayout(titleLayout);
    m_layout->addWidget(m_topBar);
}

void MainBar::setupMainBar()
{
    m_splitter = new QSplitter(this);
    m_splitter->setStyleSheet("QSplitter{"
                              "border: 0px;"
                              "}");
    QSizePolicy splSizePolicy = m_splitter->sizePolicy();
    splSizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
    m_splitter->setSizePolicy(splSizePolicy);
    m_splitter->setOrientation(Qt::Vertical);
    m_splitter->setChildrenCollapsible(false);
    m_splitter->setHandleWidth(1);

    historyFrame = new QFrame(m_splitter);
    historyFrame->setLineWidth(0);
    historyFrame->setFrameShape(QFrame::Box);
    QHBoxLayout *historyLayout = new QHBoxLayout;
    historyLayout->setAlignment(Qt::AlignLeft);
    m_chatBroswer = new QListWidget;

    historyLayout->addWidget(m_chatBroswer);
    historyFrame->setLayout(historyLayout);
    m_splitter->addWidget(historyFrame);

    chatFrame = new QFrame(m_splitter);
    chatFrame->setLineWidth(0);
    chatFrame->setFrameShape(QFrame::Box);
    chatFrame->setMinimumHeight(200);
    chatFrame->setMaximumHeight(350);
    QVBoxLayout *chatLayout = new QVBoxLayout;
    chatLayout->setContentsMargins(0,0,0,0);
    chatLayout->setAlignment(Qt::AlignTop);

    // TOOLCFRAME
    toolCFrame = new QFrame;
    toolCFrame->setFrameShape(QFrame::Box);
    toolCFrame->setLineWidth(0);
    toolCFrame->setFixedHeight(48);
    toolCFrame->setStyleSheet("QFrame QToolButton{"
                              "border: 0;"
                              "}"
                              "QFrame QToolButton:hover{"
                              "background:rgb(205,205,205);"
                              "border-radius: 10px;"
                              "border: 1px solid rgb(234,234,234);"
                              "}");
    QHBoxLayout *toolCLayout = new QHBoxLayout;
    toolCLayout->setContentsMargins(10,0,10,0);
    toolCLayout->setSpacing(10);
    toolCLayout->setAlignment(Qt::AlignLeft);
    emojiBtn = new QToolButton;
    emojiBtn->setIcon(QIcon(":/icons/emoji"));
    emojiBtn->setIconSize(QSize(32,32));

    toolCLayout->addWidget(emojiBtn);

    imageBtn = new QToolButton;
    imageBtn->setIcon(QIcon(":/icons/picture"));
    imageBtn->setIconSize(QSize(32,32));

    toolCLayout->addWidget(imageBtn);

    fileBtn = new QToolButton;
    fileBtn->setIcon(QIcon(":/icons/file"));
    fileBtn->setIconSize(QSize(32,32));

    toolCLayout->addWidget(fileBtn);
    toolCFrame->setLayout(toolCLayout);
    chatLayout->addWidget(toolCFrame);
    // END TOOLCFRAME
    chatCFrame = new QFrame;
    chatCFrame->setFrameShape(QFrame::Box);
    chatCFrame->setLineWidth(0);
    QHBoxLayout *chatCLayout = new QHBoxLayout;
    chatCLayout->setContentsMargins(10,0,10,0);

    m_chatEditor = new QTextEdit;
    chatCLayout->addWidget(m_chatEditor);
    chatCFrame->setLayout(chatCLayout);
    chatLayout->addWidget(chatCFrame);
    chatLayout->setStretchFactor(chatCFrame, 1);

    sendCFrame = new QFrame;
    sendCFrame->setFrameShape(QFrame::Box);
    sendCFrame->setLineWidth(0);
    sendCFrame->setFixedHeight(48);
    QHBoxLayout *sendCLayout = new QHBoxLayout;
    sendCLayout->setContentsMargins(10,0,10,0);
    sendCLayout->setSpacing(10);
    sendCLayout->setAlignment(Qt::AlignRight);
    sendBtn = new QPushButton("发送");
    sendCLayout->addWidget(sendBtn);
    sendCFrame->setLayout(sendCLayout);
    chatLayout->addWidget(sendCFrame);

    chatFrame->setLayout(chatLayout);

    m_splitter->addWidget(chatFrame);
    m_layout->addWidget(m_splitter);
}

void MainBar::changeBar(QString name, QString ip)
{
    m_title=name+ip;
    QLabel *title = m_topBar->findChild<QLabel*>();
    title->setText(m_title);
    clearBroswer();
}

void MainBar::addMessage(message msg)
{
    QString time = msg.time;
    dealMessageTime(time);
    QString message = msg.msg;
    QString ip = msg.ip;
    MYLOG<<time<<" "<<message<<" "<<ip;
    QNChatMessage* messageW = new QNChatMessage(m_chatBroswer->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(m_chatBroswer);

    if(ip==m_profile->m_ip){
        dealMessage(messageW, item, message, time, QNChatMessage::User_Me);
    }else{
        dealMessage(messageW, item, message, time, QNChatMessage::User_Other);
    }

    m_chatBroswer->setCurrentRow(m_chatBroswer->count()-1);
}

void MainBar::addMessages(Message msgs)
{

}

void MainBar::clearBroswer()
{
    m_chatBroswer->clear();
    m_chatEditor->clear();
}

void MainBar::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time,  QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(m_chatBroswer->width()-20);
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(text, time, size, type);
    m_chatBroswer->setItemWidget(item, messageW);
}

void MainBar::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if(m_chatBroswer->count() > 0) {
        QListWidgetItem* lastItem = m_chatBroswer->item(m_chatBroswer->count() - 1);
        QNChatMessage* messageW = (QNChatMessage*)m_chatBroswer->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        MYLOG << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        QNChatMessage* messageTime = new QNChatMessage(m_chatBroswer->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(m_chatBroswer);

        QSize size = QSize(m_chatBroswer->width()-20,40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
        m_chatBroswer->setItemWidget(itemTime, messageTime);
    }
}

