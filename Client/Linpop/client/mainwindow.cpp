#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qtmaterialavatar.h>
#include <qtmaterialbadge.h>
#include <qtmaterialiconbutton.h>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QSizePolicy>
#include <QStackedLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTextEdit>
#include <QSplitter>
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      m_avatar(new QtMaterialAvatar(QImage(":/icons/avatar"))),
      m_badge(new QtMaterialBadge)
{
    makeFakeData();

    m_avatar->setSize(48);
    connect(m_avatar, &QtMaterialAvatar::clicked, [=](){
        qDebug()<<"avatar clicked";
    });

    // WINDOW SETTING
    resize(1210,760);
    setFixedSize(1210,760);
    setWindowTitle("Linpop");

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->setSpacing(0);

    QFrame *toolFrame = leftUi();
    QFrame *listFrame = middleUi();
    QFrame *mainFrame = rightUi();

    // ADD FRAME TO LAYOUT
    mainLayout->addWidget(toolFrame,0,0);
    mainLayout->addWidget(listFrame, 0,1);
    mainLayout->addWidget(mainFrame,0,2);


    // SET LAYOUT TO CENTRALWIDGET
    QWidget *mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QFrame *MainWindow::leftUi()
{
    // TOOLFRAME in LEFT
    QFrame *toolFrame = new QFrame();
    toolFrame->setFrameStyle(QFrame::Box);
    toolFrame->setLineWidth(1);
    toolFrame->setFixedWidth(72);
    QVBoxLayout *toolLayout = new QVBoxLayout();
    toolLayout->setContentsMargins(0,0,0,0);
    toolLayout->setSpacing(0);

    // AVATARFRAME
    QFrame *avatarTFrame = new QFrame();
    avatarTFrame->setFrameStyle(QFrame::Box);
    avatarTFrame->setLineWidth(1);
    avatarTFrame->setFixedHeight(54);
    QHBoxLayout *avatarTLayout = new QHBoxLayout();
    avatarTLayout->setContentsMargins(0,0,0,0);
    avatarTLayout->setSpacing(0);
    avatarTLayout->addWidget(m_avatar);
    //END AVATARFRAME
    avatarTFrame->setLayout(avatarTLayout);
    toolLayout->addWidget(avatarTFrame);

    // ACTIONFRAME
    QFrame *actionTFrame = new QFrame();
    actionTFrame->setFrameStyle(QFrame::Box);
    QVBoxLayout *actionTLayout = new QVBoxLayout();
    actionTLayout->setContentsMargins(0,12,0,12);
    actionTLayout->setSpacing(12);
    actionTLayout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

    QToolButton *messageBtn = new QToolButton();
    messageBtn->setIcon(QIcon(":/icons/chat"));
    messageBtn->setFixedSize(QSize(48,48));
    messageBtn->setIconSize(QSize(32,32));
    connect(messageBtn, &QToolButton::clicked, [&](){
        emit switched(0);
    });
    actionTLayout->addWidget(messageBtn);
    QToolButton *contactBtn = new QToolButton();
    contactBtn->setIcon(QIcon(":/icons/person"));
    contactBtn->setFixedSize(QSize(48,48));
    contactBtn->setIconSize(QSize(32,32));
    actionTLayout->addWidget(contactBtn);
    actionTLayout->addStretch();
    connect(contactBtn, &QToolButton::clicked, [&](){
        emit switched(1);
    });
    QToolButton *settingBtn = new QToolButton();
    settingBtn->setIcon(QIcon(":/icons/setting"));
    settingBtn->setFixedSize(QSize(48,48));
    settingBtn->setIconSize(QSize(32,32));
    actionTLayout->addWidget(settingBtn);
    // END ACTIONFRAME
    actionTFrame->setLayout(actionTLayout);
    toolLayout->addWidget(actionTFrame);

    // END TOOLFRAME in LEFT
    toolFrame->setLayout(toolLayout);
    return toolFrame;
}

QFrame *MainWindow::middleUi()
{
    // LISTFRAME in MIDDLE
    QFrame *listFrame = new QFrame;
    listFrame->setFrameStyle(QFrame::Box);
    listFrame->setLineWidth(1);
    listFrame->setFixedWidth(308);
    QVBoxLayout *listLayout = new QVBoxLayout();
    listLayout->setContentsMargins(0,0,0,0);
    listLayout->setSpacing(0);

    // ACTIONFRAME
    QFrame *actionLFrame = new QFrame;
    actionLFrame->setFrameStyle(QFrame::Box);
    actionLFrame->setLineWidth(1);
    actionLFrame->setFixedHeight(54);
    QHBoxLayout *actionLLayout = new QHBoxLayout();
    actionLLayout->setContentsMargins(8,0,8,0);
    actionLLayout->setSpacing(5);
    actionLLayout->setAlignment(Qt::AlignLeft);

    QLineEdit *searchBar = new QLineEdit();
    searchBar->setPlaceholderText("搜索");
    searchBar->setFixedHeight(32);
    actionLLayout->addWidget(searchBar);
    QToolButton *addContactBtn = new QToolButton;
    addContactBtn->setIcon(QIcon(":/icons/add-person"));
    addContactBtn->setFixedSize(QSize(32,32));
    addContactBtn->setIconSize(QSize(16,16));
    actionLLayout->addWidget(addContactBtn);

    // END ACTIONLFRAME
    actionLFrame->setLayout(actionLLayout);
    listLayout->addWidget(actionLFrame);

    // STACKFRAME
    QFrame *stackLFrame = new QFrame;
    stackLFrame->setFixedWidth(listFrame->width()-2);
    stackLFrame->setFrameStyle(QFrame::Box);
    stackLFrame->setLineWidth(1);
    QStackedLayout *stackLLayout = new QStackedLayout;
    stackLLayout->setContentsMargins(0,0,0,0);
    stackLLayout->setSpacing(0);

    // MESSAGE LIST
    QListWidget *messageWidget = new QListWidget;
    messageWidget->setFixedWidth(listFrame->width()-2);
    for (int i=0; i<fakeListData->size();i++) {
        QFrame *messageItemFrame = new QFrame;
        messageItemFrame->setFrameShape(QFrame::Box);
        messageItemFrame->setLineWidth(1);
        messageItemFrame->setFixedHeight(90);
        QHBoxLayout *messageItemLayout = new QHBoxLayout;

        QLabel *avatar = new QLabel;
        avatar->setStyleSheet("QLabel{"
                              "border-radius:30px;"
                              "border-image:url(:/icons/avatar);"
                              "}");
        avatar->setFixedSize(60,60); messageItemLayout->addWidget(avatar);

        QVBoxLayout *rightMILayout = new QVBoxLayout;
        rightMILayout->setContentsMargins(0,0,0,0);
        QLabel *msg = new QLabel(fakeListData->at(i).toObject()["name"].toString()+"("+fakeListData->at(i).toObject()["ip"].toString()+")");
        rightMILayout->addWidget(msg);

        messageItemLayout->addLayout(rightMILayout);
        messageItemFrame->setLayout(messageItemLayout);

        QListWidgetItem *messageItem = new QListWidgetItem;
        messageItem->setSizeHint(QSize(messageWidget->width()-2,90));
        messageWidget->addItem(messageItem);
        messageWidget->setItemWidget(messageItem, messageItemFrame);
    }
    stackLLayout->addWidget(messageWidget);
    //END MESSAGE LIST

    // CONTACT FRAME
    QFrame *contactFrame = new QFrame;
    contactFrame->setFixedWidth(308);
    QVBoxLayout *contactLayout = new QVBoxLayout;
    contactLayout->setContentsMargins(10,0,10,0);
    contactLayout->setAlignment(Qt::AlignTop);

    // TAB FRAME
    QFrame *tabCFrame = new QFrame;
    tabCFrame->setFixedHeight(72);
    QHBoxLayout *tabCLayout = new QHBoxLayout;
    tabCLayout->setContentsMargins(0,0,0,0);
    QPushButton *personBtn = new QPushButton("好友");
    tabCLayout->addWidget(personBtn);
    QPushButton *groupBtn = new QPushButton("群聊");
    tabCLayout->addWidget(groupBtn);
    tabCFrame->setLayout(tabCLayout);
    contactLayout->addWidget(tabCFrame);
    // END TAB FRAME

    // CONTACT LIST
    QFrame *stackCFrame = new QFrame;
    stackCFrame->setFixedWidth(contactFrame->width()-20);
    stackCFrame->setFrameStyle(QFrame::Box);
    stackCFrame->setLineWidth(1);
    QStackedLayout *stackCLayout = new QStackedLayout;
    stackCLayout->setContentsMargins(0,0,0,0);
    stackCLayout->setSpacing(0);

    QListWidget *personList = new QListWidget;
    for (int i=0; i<fakeListData->size();i++) {
        QFrame *contactItemFrame = new QFrame;
        contactItemFrame->setFrameShape(QFrame::Box);
        contactItemFrame->setLineWidth(1);
        contactItemFrame->setFixedHeight(90);
        QHBoxLayout *contactItemLayout = new QHBoxLayout;

        QLabel *avatar = new QLabel;
        avatar->setStyleSheet("QLabel{"
                              "border-radius:2px;"
                              "border-image:url(:/icons/person);"
                              "}");
        avatar->setFixedSize(60,60); contactItemLayout->addWidget(avatar);

        QVBoxLayout *rightCILayout = new QVBoxLayout;
        rightCILayout->setContentsMargins(0,0,0,0);
        QLabel *msg = new QLabel(fakeListData->at(i).toObject()["name"].toString()+"("+fakeListData->at(i).toObject()["ip"].toString()+")");
        rightCILayout->addWidget(msg);

        contactItemLayout->addLayout(rightCILayout);
        contactItemFrame->setLayout(contactItemLayout);

        QListWidgetItem *contactItem = new QListWidgetItem;
        contactItem->setSizeHint(QSize(personList->width()-2,90));
        personList->addItem(contactItem);
        personList->setItemWidget(contactItem, contactItemFrame);
    }

    stackCLayout->addWidget(personList);

    stackCFrame->setLayout(stackCLayout);
    contactLayout->addWidget(stackCFrame);
    // END CONTACT LIST

    contactFrame->setLayout(contactLayout);
    stackLLayout->addWidget(contactFrame);
    // END CONTACT FRAME

    // END STACKFRAME
    connect(this, &MainWindow::switched, [=](int index){
        stackLLayout->setCurrentIndex(index);
    });
    stackLFrame->setLayout(stackLLayout);

    // END LISTFRAME
    listLayout->addWidget(stackLFrame);

    // END LISTFRAME in MIDDLE
    listFrame->setLayout(listLayout);
    return listFrame;
}

QFrame *MainWindow::rightUi()
{
    // MAINFRAME in RIGHT
    QFrame *mainFrame = new QFrame;
    mainFrame->setFrameStyle(QFrame::Box);
    mainFrame->setLineWidth(1);
    QSizePolicy mFSizePolicy = mainFrame->sizePolicy();
    mFSizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    mainFrame->setSizePolicy(mFSizePolicy);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    QFrame *titleFrame = new QFrame;
    titleFrame->setFixedHeight(72);
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setAlignment(Qt::AlignLeft);
    QLabel *title =new QLabel("name");
    titleLayout->addWidget(title);
    titleFrame->setLayout(titleLayout);
    mainLayout->addWidget(titleFrame);


    QSplitter *splitter = new QSplitter(mainFrame);
    splitter->setStyleSheet("QSplitter{"
                            "border: 1px solid;"
                            "}");
    QSizePolicy splSizePolicy = splitter->sizePolicy();
    splSizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
    splitter->setSizePolicy(splSizePolicy);
    splitter->setOrientation(Qt::Vertical);
    splitter->setChildrenCollapsible(false);
    splitter->setHandleWidth(1);

    QFrame *historyFrame = new QFrame(splitter);
    historyFrame->setFrameShape(QFrame::Box);
    QHBoxLayout *historyLayout = new QHBoxLayout;
    historyLayout->setAlignment(Qt::AlignLeft);
    QListWidget *historyList = new QListWidget;
    chatBroswer = historyList;

    historyLayout->addWidget(chatBroswer);
    historyFrame->setLayout(historyLayout);
    splitter->addWidget(historyFrame);

    QFrame *chatFrame = new QFrame(splitter);
    chatFrame->setFrameShape(QFrame::Box);
    chatFrame->setMinimumHeight(200);
    chatFrame->setMaximumHeight(350);
    QVBoxLayout *chatLayout = new QVBoxLayout;
    chatLayout->setContentsMargins(0,0,0,0);
    chatLayout->setAlignment(Qt::AlignTop);

    // TOOLCFRAME
    QFrame *toolCFrame = new QFrame;
    toolCFrame->setFrameShape(QFrame::Box);
    toolCFrame->setLineWidth(1);
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
    chatCFrame->setLineWidth(1);
    QHBoxLayout *chatCLayout = new QHBoxLayout;
    chatCLayout->setContentsMargins(10,0,10,0);

    QTextEdit *chateditor = new QTextEdit;
    chatEditor = chateditor;
    chatCLayout->addWidget(chateditor);
    chatCFrame->setLayout(chatCLayout);
    chatLayout->addWidget(chatCFrame);
    chatLayout->setStretchFactor(chatCFrame, 1);

    QFrame *sendCFrame = new QFrame;
    sendCFrame->setFrameShape(QFrame::Box);
    sendCFrame->setLineWidth(1);
    sendCFrame->setFixedHeight(48);
    QHBoxLayout *sendCLayout = new QHBoxLayout;
    sendCLayout->setContentsMargins(10,0,10,0);
    sendCLayout->setSpacing(10);
    sendCLayout->setAlignment(Qt::AlignRight);
    QPushButton *sendBtn = new QPushButton("发送");
    connect(sendBtn, &QPushButton::clicked, [&](){
        on_pushButton_clicked();
    });
    sendCLayout->addWidget(sendBtn);
    sendCFrame->setLayout(sendCLayout);
    chatLayout->addWidget(sendCFrame);
    // END SENDCFRAME

    chatFrame->setLayout(chatLayout);

    splitter->addWidget(chatFrame);
    mainLayout->addWidget(splitter);
    mainFrame->setLayout(mainLayout);
    return mainFrame;
}

void MainWindow::makeFakeData()
{
    fakeListData = new QJsonArray;
    QVector<QString> ips = {"127.0.0.1","127.0.0.2","126.0.0.3"};
    QVector<QString> names = {"张三","李四","王五"};
    for(int i=0;i<ips.length();i++){
        QJsonObject obj;
        obj.insert("name",names[i]);
        obj.insert("ip",ips[i]);
        fakeListData->append(obj);
    }
    QJsonDocument document;
    document.setArray(*fakeListData);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    qDebug().noquote() << strJson;
}

void MainWindow::on_pushButton_clicked()
{
    QString msg = chatEditor->toPlainText();
    chatEditor->setText("");
    QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

    bool isSending = true; // 发送中

    qDebug()<<"addMessage" << msg << time << chatBroswer->count();
    if(chatBroswer->count()%2) {
        if(isSending) {
            dealMessageTime(time);

            QNChatMessage* messageW = new QNChatMessage(chatBroswer->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(chatBroswer);
            dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
        } else {
            bool isOver = true;
            for(int i = chatBroswer->count() - 1; i > 0; i--) {
                QNChatMessage* messageW = (QNChatMessage*)chatBroswer->itemWidget(chatBroswer->item(i));
                if(messageW->text() == msg) {
                    isOver = false;
                    messageW->setTextSuccess();
                }
            }
            if(isOver) {
                dealMessageTime(time);

                QNChatMessage* messageW = new QNChatMessage(chatBroswer->parentWidget());
                QListWidgetItem* item = new QListWidgetItem(chatBroswer);
                dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
                messageW->setTextSuccess();
            }
        }
    } else {
        if(msg != "") {
            dealMessageTime(time);

            QNChatMessage* messageW = new QNChatMessage(chatBroswer->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(chatBroswer);
            dealMessage(messageW, item, msg, time, QNChatMessage::User_She);
        }
    }
    chatBroswer->setCurrentRow(chatBroswer->count()-1);
}

void MainWindow::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time,  QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(this->width());
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(text, time, size, type);
    chatBroswer->setItemWidget(item, messageW);
}

void MainWindow::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if(chatBroswer->count() > 0) {
        QListWidgetItem* lastItem = chatBroswer->item(chatBroswer->count() - 1);
        QNChatMessage* messageW = (QNChatMessage*)chatBroswer->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
//        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        QNChatMessage* messageTime = new QNChatMessage(chatBroswer->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(chatBroswer);

        QSize size = QSize(this->width(), 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
        chatBroswer->setItemWidget(itemTime, messageTime);
    }
}

