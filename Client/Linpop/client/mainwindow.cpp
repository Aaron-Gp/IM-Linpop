#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qtmaterialavatar.h>
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
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    m_profile = ProfileManager::getInstance();
    setStyleSheet("QMainWindow{"
                  "background:rgb(234,234,234);"
                  "margin:0;"
                  "padding:0;"
                  "}");
    m_avatar = new QtMaterialAvatar(QImage(":/icons/avatar"));
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
    mainLayout->setContentsMargins(0,0,0,0);

    m_sideBar = new SideBar(this);
    m_listBar = new ListBar(this);
    m_mainBar = new MainBar(this);

    // ADD FRAME TO LAYOUT
    mainLayout->addWidget(m_sideBar,0,0);
    mainLayout->addWidget(m_listBar,0,1);
    mainLayout->addWidget(m_mainBar,0,2);

    // 列表面板切换
    for(int i=0;i<m_sideBar->toolVector.size();i++){
        connect(m_sideBar->toolVector[i],&QToolButton::clicked,[=](){
            m_listBar->stackLLayout->setCurrentIndex(i);
        });
    }

    // 聊天面板切换
    connect(m_listBar->messageWidget,&QListWidget::currentRowChanged,[&](int row){
        QString name = m_listBar->fakeListData->at(row).toObject()["name"].toString();
        QString ip = m_listBar->fakeListData->at(row).toObject()["ip"].toString();
        m_mainBar->changeBar(name, ip);
    });

    // client连接
    m_client = new TcpClient;
    connect(m_listBar->addContactBtn, &QToolButton::clicked, [&](){
        QString ip = m_listBar->searchBar->text();
        m_client->tcpClient->connectToHost(ip, 8000);
        bool ok = m_client->tcpClient->waitForConnected(1000);
        if (ok)
        {
            QString data = "hello from client!";
            m_client->tcpClient->write(data.toUtf8());
            Dict newContact;
            newContact.insert("ip", m_client->tcpClient->peerAddress().toString().split("::ffff:")[1]+m_client->tcpClient->peerPort());
            m_profile->m_contact.append(newContact);
        }else{
            qDebug()<<"fail to connect "<<ip;
        }
    });


    // server启动
    m_server = new TcpServer;
    qDebug()<<QNetworkInterface().allAddresses();
    m_listBar->m_localIpLabel->setText(QNetworkInterface().allAddresses().at(2).toString()); // 存储本地ip
    m_profile->m_ip = QNetworkInterface().allAddresses().at(2).toString() + ":8000"; // 存储本地ip
    connect(m_listBar->portSubmitBtn, &QPushButton::clicked, [&](){
        if(m_listBar->portSubmitBtn->text()=="连接"){
            bool ok = m_server->tcpServer->listen(QHostAddress::Any, m_listBar->m_serverPortEditor->text().toInt());
            if(ok)
            {
                m_listBar->portSubmitBtn->setText("断开");
                qDebug()<<"server listen on"<< m_listBar->m_serverPortEditor->text();
            }
        }else{
            m_server->closeAll();
            m_listBar->portSubmitBtn->setText("连接");
            qDebug()<<"server closed";
        }

    });

    // SET LAYOUT TO CENTRALWIDGET
    QWidget *mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
    m_server->closeAll();
}


