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
    setLayout(m_layout);

//    setupUi();
}

void MainBar::setupUi()
{
    setupTopBar();
    setupMainBar();
}

void MainBar::setupTopBar()
{
    m_topBar = new QFrame;
    m_topBar->setLineWidth(0);
    m_topBar->setFixedHeight(72);
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

    QFrame *historyFrame = new QFrame(m_splitter);
    historyFrame->setLineWidth(0);
    historyFrame->setFrameShape(QFrame::Box);
    QHBoxLayout *historyLayout = new QHBoxLayout;
    historyLayout->setAlignment(Qt::AlignLeft);
    QListWidget *historyList = new QListWidget;
    m_chatBroswer = historyList;

    historyLayout->addWidget(m_chatBroswer);
    historyFrame->setLayout(historyLayout);
    m_splitter->addWidget(historyFrame);

    QFrame *chatFrame = new QFrame(m_splitter);
    chatFrame->setLineWidth(0);
    chatFrame->setFrameShape(QFrame::Box);
    chatFrame->setMinimumHeight(200);
    chatFrame->setMaximumHeight(350);
    QVBoxLayout *chatLayout = new QVBoxLayout;
    chatLayout->setContentsMargins(0,0,0,0);
    chatLayout->setAlignment(Qt::AlignTop);

    // TOOLCFRAME
    QFrame *toolCFrame = new QFrame;
    toolCFrame->setFrameShape(QFrame::Box);
    toolCFrame->setLineWidth(0);
    toolCFrame->setFixedHeight(48);
    QHBoxLayout *toolCLayout = new QHBoxLayout;
    toolCLayout->setContentsMargins(10,0,10,0);
    toolCLayout->setSpacing(10);
    toolCLayout->setAlignment(Qt::AlignLeft);
    QToolButton *emojiBtn = new QToolButton;
    emojiBtn->setIcon(QIcon(":/icons/emoji"));
    emojiBtn->setIconSize(QSize(32,32));

    toolCLayout->addWidget(emojiBtn);

    QToolButton *imageBtn = new QToolButton;
    imageBtn->setIcon(QIcon(":/icons/picture"));
    imageBtn->setIconSize(QSize(32,32));

    toolCLayout->addWidget(imageBtn);

    QToolButton *fileBtn = new QToolButton;
    fileBtn->setIcon(QIcon(":/icons/file"));
    fileBtn->setIconSize(QSize(32,32));

    toolCLayout->addWidget(fileBtn);
    toolCFrame->setLayout(toolCLayout);
    chatLayout->addWidget(toolCFrame);
    // END TOOLCFRAME

    QFrame *chatCFrame = new QFrame;
    chatCFrame->setFrameShape(QFrame::Box);
    chatCFrame->setLineWidth(0);
    QHBoxLayout *chatCLayout = new QHBoxLayout;
    chatCLayout->setContentsMargins(10,0,10,0);

    QTextEdit *chateditor = new QTextEdit;
    m_chatEditor = chateditor;
    chatCLayout->addWidget(chateditor);
    chatCFrame->setLayout(chatCLayout);
    chatLayout->addWidget(chatCFrame);
    chatLayout->setStretchFactor(chatCFrame, 1);

    QFrame *sendCFrame = new QFrame;
    sendCFrame->setFrameShape(QFrame::Box);
    sendCFrame->setLineWidth(0);
    sendCFrame->setFixedHeight(48);
    QHBoxLayout *sendCLayout = new QHBoxLayout;
    sendCLayout->setContentsMargins(10,0,10,0);
    sendCLayout->setSpacing(10);
    sendCLayout->setAlignment(Qt::AlignRight);
    QPushButton *sendBtn = new QPushButton("发送");
    connect(sendBtn, &QPushButton::clicked, [&](){
        QString msg = m_chatEditor->toPlainText();
        m_chatEditor->setText("");
        QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

        bool isSending = true; // 发送中

        qDebug()<<"addMessage" << msg << time << m_chatBroswer->count();
        if(m_chatBroswer->count()%2) {
            if(isSending) {
                dealMessageTime(time);

                QNChatMessage* messageW = new QNChatMessage(m_chatBroswer->parentWidget());
                QListWidgetItem* item = new QListWidgetItem(m_chatBroswer);
                dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
            } else {
                bool isOver = true;
                for(int i = m_chatBroswer->count() - 1; i > 0; i--) {
                    QNChatMessage* messageW = (QNChatMessage*)m_chatBroswer->itemWidget(m_chatBroswer->item(i));
                    if(messageW->text() == msg) {
                        isOver = false;
                        messageW->setTextSuccess();
                    }
                }
                if(isOver) {
                    dealMessageTime(time);

                    QNChatMessage* messageW = new QNChatMessage(m_chatBroswer->parentWidget());
                    QListWidgetItem* item = new QListWidgetItem(m_chatBroswer);
                    dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
                    messageW->setTextSuccess();
                }
            }
        } else {
            if(msg != "") {
                dealMessageTime(time);

                QNChatMessage* messageW = new QNChatMessage(m_chatBroswer->parentWidget());
                QListWidgetItem* item = new QListWidgetItem(m_chatBroswer);
                dealMessage(messageW, item, msg, time, QNChatMessage::User_She);
            }
        }
        m_chatBroswer->setCurrentRow(m_chatBroswer->count()-1);
    });
    sendCLayout->addWidget(sendBtn);
    sendCFrame->setLayout(sendCLayout);
    chatLayout->addWidget(sendCFrame);

    chatFrame->setLayout(chatLayout);

    m_splitter->addWidget(chatFrame);
    m_layout->addWidget(m_splitter);
}

void MainBar::changeBar(QString name, QString ip)
{
    if(m_title==""){
        m_title=name+ip;
        setupUi();
    }
    else{
        m_title=name+ip;
        QLabel *title = m_topBar->findChild<QLabel*>();
        title->setText(m_title);
    }
}

void MainBar::addMessage(QJsonObject msg)
{
    QString time = QString::number(QDateTime::currentDateTime().toTime_t());
    dealMessageTime(time);
    QString message = msg["msg"].toString();
    QString ip = msg["ip"].toString();
    QNChatMessage* messageW = new QNChatMessage(m_chatBroswer->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(m_chatBroswer);
    dealMessage(messageW, item, message, time, QNChatMessage::User_She);

    m_chatBroswer->setCurrentRow(m_chatBroswer->count()-1);
}

void MainBar::addMessages(QJsonArray msgs)
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
        qDebug() << "curTime lastTime:" << curTime - lastTime;
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

