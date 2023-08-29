#include "listbar.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>
#include <QStackedLayout>
#include <QListWidget>
#include <QListWidgetItem>

ListBar::ListBar(QWidget *parent) : QFrame(parent)
{
    setFrameStyle(QFrame::Box);
    setLineWidth(0);
    setFixedWidth(308);
    m_layout = new QVBoxLayout();
    m_layout->setAlignment(Qt::AlignTop);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);
    m_profile = ProfileManager::getInstance();
    makeFakeData();
    setupUi();
}

void ListBar::addContact(profile pf)
{
    // ADD TO MESSAGE LIST
    QFrame *messageItemFrame = new QFrame;
    messageItemFrame->setFrameShape(QFrame::Box);
    messageItemFrame->setLineWidth(0);
    messageItemFrame->setFixedHeight(90);
    QHBoxLayout *messageItemLayout = new QHBoxLayout;
    QLabel *avatar = new QLabel;
    avatar->setStyleSheet("QLabel{"
                          "border-radius:30px;"
                          "border-image:url("+pf.avatar+");"
                          "}");
    avatar->setFixedSize(60,60);
    messageItemLayout->addWidget(avatar);

    QVBoxLayout *rightMILayout = new QVBoxLayout;
    rightMILayout->setContentsMargins(0,0,0,0);
    QLabel *msg = new QLabel(pf.name+"("+pf.ip+")");
    rightMILayout->addWidget(msg);

    messageItemLayout->addLayout(rightMILayout);
    messageItemFrame->setLayout(messageItemLayout);

    QListWidgetItem *messageItem = new QListWidgetItem;

    messageItem->setSizeHint(QSize(messageWidget->width(),90));
    messageWidget->addItem(messageItem);
    messageWidget->setItemWidget(messageItem, messageItemFrame);


    // ADD TO CONTACT LIST
    QFrame *contactItemFrame = new QFrame;
    contactItemFrame->setFrameShape(QFrame::Box);
    contactItemFrame->setLineWidth(0);
    contactItemFrame->setFixedHeight(90);
    QHBoxLayout *contactItemLayout = new QHBoxLayout;

    avatar = new QLabel;
    avatar->setStyleSheet("QLabel{"
                          "border-radius:2px;"
                          "border-image:url("+pf.avatar+");"
                          "}");
    avatar->setFixedSize(60,60); contactItemLayout->addWidget(avatar);

    QVBoxLayout *rightCILayout = new QVBoxLayout;
    rightCILayout->setContentsMargins(0,0,0,0);
    msg = new QLabel(pf.name+"("+pf.ip+")");
    rightCILayout->addWidget(msg);

    contactItemLayout->addLayout(rightCILayout);
    contactItemFrame->setLayout(contactItemLayout);

    QListWidgetItem *contactItem = new QListWidgetItem;
    contactItem->setSizeHint(QSize(personList->width()-2,90));
    personList->addItem(contactItem);
    personList->setItemWidget(contactItem, contactItemFrame);
}

void ListBar::setupUi()
{
    setupTopBar();
    setupStackBar();
}

void ListBar::setupTopBar()
{
    m_topBar = new QFrame;
    m_topBar->setStyleSheet("QFrame{"
                            "background:white;"
                            "}");
    m_topBar->setFrameStyle(QFrame::Box);
    m_topBar->setLineWidth(0);
    m_topBar->setFixedHeight(54);
    QHBoxLayout *actionLLayout = new QHBoxLayout();
    actionLLayout->setContentsMargins(8,0,8,0);
    actionLLayout->setSpacing(5);
    actionLLayout->setAlignment(Qt::AlignLeft);

    searchBar = new QLineEdit();
    searchBar->setPlaceholderText("添加好友ip");
    searchBar->setFixedHeight(32);
    actionLLayout->addWidget(searchBar);
    addContactBtn = new QToolButton;
    addContactBtn->setIcon(QIcon(":/icons/add-person"));
    addContactBtn->setFixedSize(QSize(32,32));
    addContactBtn->setIconSize(QSize(16,16));
    actionLLayout->addWidget(addContactBtn);

    m_topBar->setLayout(actionLLayout);
    m_layout->addWidget(m_topBar);
}

void ListBar::setupStackBar()
{
    QFrame *stackLFrame = new QFrame;
    stackLFrame->setStyleSheet(".QFrame{"
                               "border: 0;"
                               "background:white;"
                               "}");
    stackLFrame->setFixedWidth(width());
    stackLFrame->setFrameStyle(QFrame::Box);
    stackLFrame->setLineWidth(0);
    stackLLayout = new QStackedLayout;
    stackLLayout->setContentsMargins(0,0,0,0);
    stackLLayout->setSpacing(0);

    // MESSAGE LIST
    messageWidget = new QListWidget;
    messageWidget->setFixedWidth(width());
    for (int i=0; i<fakeListData->size();i++) {
        QFrame *messageItemFrame = new QFrame;
        messageItemFrame->setFrameShape(QFrame::Box);
        messageItemFrame->setLineWidth(0);
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

        messageItem->setSizeHint(QSize(messageWidget->width(),90));
        messageWidget->addItem(messageItem);
        messageWidget->setItemWidget(messageItem, messageItemFrame);
    }

    connect(messageWidget, &QListWidget::currentItemChanged,[&](QListWidgetItem *cur, QListWidgetItem *pre){
        QWidget* pwig = messageWidget->itemWidget(cur);
        if (pwig != NULL)
        {
            pwig->setStyleSheet("QFrame{"
                                "background:rgb(0,153,255);"
                                "color:white;"
                                "}");
        }
        pwig = messageWidget->itemWidget(pre);
        if (pwig != NULL)
        {
            pwig->setStyleSheet("QFrame{"
                                "background:white;"
                                "color:black;"
                                "}");
        }
    });
    stackLLayout->addWidget(messageWidget);
    //END MESSAGE LIST

    // CONTACT FRAME
    contactFrame = new QFrame;
    contactFrame->setFrameShape(QFrame::Box);
    contactFrame->setLineWidth(0);
    contactFrame->setFixedWidth(308);
    QVBoxLayout *contactLayout = new QVBoxLayout;
    contactLayout->setContentsMargins(10,0,10,0);
    contactLayout->setAlignment(Qt::AlignTop);

    // TAB FRAME
    tabCFrame = new QFrame;
    tabCFrame->setFixedHeight(72);
    QHBoxLayout *tabCLayout = new QHBoxLayout;
    tabCLayout->setContentsMargins(10,0,10,0);
    tabCLayout->setSpacing(20);
    tabCFrame->setStyleSheet(".QFrame{"
                             "background:white;"
                             "margin: 0px 0px 0px 0px;"
                             "}"
                             ".QFrame .QPushButton{"
                             "background: rgb(234,234,234);"
                             "border: 0;"
                             "height: 48px;"
                             "}"
                             ".QFrame .QPushButton:hover{"
                             "background:rgb(234,234,234);"
                             "color:rgb(0,147,255);"
                             "border: 1px solid rgb(234,234,234);"
                             "}");
    QPushButton *personBtn = new QPushButton("好友");
    tabCLayout->addWidget(personBtn);
    QPushButton *groupBtn = new QPushButton("群聊");
    tabCLayout->addWidget(groupBtn);
    tabCFrame->setLayout(tabCLayout);
    contactLayout->addWidget(tabCFrame);
    // END TAB FRAME

    // CONTACT LIST
    stackCFrame = new QFrame;
    stackCFrame->setFixedWidth(contactFrame->width()-20);
    stackCFrame->setFrameStyle(QFrame::Box);
    stackCFrame->setLineWidth(0);
    QStackedLayout *stackCLayout = new QStackedLayout;
    stackCLayout->setContentsMargins(0,0,0,0);
    stackCLayout->setSpacing(0);

    personList = new QListWidget;
    for (int i=0; i<fakeListData->size();i++) {
        QFrame *contactItemFrame = new QFrame;
        contactItemFrame->setFrameShape(QFrame::Box);
        contactItemFrame->setLineWidth(0);
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
    contactFrame->setLayout(contactLayout);
    stackLLayout->addWidget(contactFrame);
    // END CONTACT LIST

    // p2p面板
    QFrame *p2pFrame = new QFrame;
    p2pFrame->setFrameShape(QFrame::Box);
    p2pFrame->setLineWidth(0);
    p2pFrame->setFixedWidth(308);
    QVBoxLayout *p2pLayout = new QVBoxLayout;
    p2pLayout->setContentsMargins(10,0,10,0);
    p2pLayout->setAlignment(Qt::AlignTop);
    m_localIpLabel = new QLabel("");
    p2pLayout->addWidget(m_localIpLabel);
    m_serverPortEditor = new QLineEdit;
    m_serverPortEditor->setPlaceholderText("请输入服务监听端口");
    p2pLayout->addWidget(m_serverPortEditor);
    portSubmitBtn = new QPushButton("连接");
    p2pLayout->addWidget(portSubmitBtn);

    p2pFrame->setLayout(p2pLayout);
    stackLLayout->addWidget(p2pFrame);

    stackLFrame->setLayout(stackLLayout);
    m_layout->addWidget(stackLFrame);
}

void ListBar::makeFakeData()
{
    fakeListData = new QJsonArray;
    QVector<QString> ips = {};
    QVector<QString> names = {};
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

//    qDebug().noquote() << strJson;
}
