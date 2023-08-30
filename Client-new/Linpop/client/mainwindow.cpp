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
#include "changeheaderwnd.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    m_profile = ProfileManager::getInstance();
    m_analyzer=MsgAnalyzer::getInstance();
    setStyleSheet("QMainWindow{"
                  "background:rgb(234,234,234);"
                  "margin:0;"
                  "padding:0;"
                  "}");

    // WINDOW SETTING
    resize(1210,760);
    setFixedSize(1210,760);
    setWindowTitle("Linpop");


}

MainWindow::~MainWindow()
{
    delete ui;
    m_server->closeAll();
}

void MainWindow::setUpUi()
{
    mainLayout = new QGridLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    m_sideBar = new SideBar(this);
    m_listBar = new ListBar(this);
    m_mainBar = new MainBar(this);

    // ADD FRAME TO LAYOUT
    mainLayout->addWidget(m_sideBar,0,0);
    mainLayout->addWidget(m_listBar,0,1);
    mainLayout->addWidget(m_mainBar,0,2);

    MYLOG<<"frame added!";

    // 列表面板切换
    for(int i=0;i<m_sideBar->toolVector.size();i++){
        connect(m_sideBar->toolVector[i],&QToolButton::clicked,[=](){
            m_listBar->stackLLayout->setCurrentIndex(i);
        });
    }

    // 聊天面板切换 checked
    connect(m_listBar->messageWidget,&QListWidget::currentRowChanged,[&](int row){
        QString id = m_profile->m_contact[row];
        m_mainBar->changeBar(id);
    });

    MYLOG<<"actions initialized!";

    // client连接
    m_client = new TcpClient;
    connect(m_listBar->addContactBtn, &QToolButton::clicked, [&](){
        QString ip = m_listBar->searchBar->text();
        bool success = m_client->newConnection(ip);
        MYLOG<<"success"<<success<<"contains? "<<ip;
        /*if(success && !m_profile->m_contact.contains(ip)){
            m_profile->m_contact.append(ip);
            // 列表增加一项
            m_listBar->addContact(m_profile->m_contactProfile.last());
            m_listBar->messageWidget->setCurrentRow(m_listBar->messageWidget->count()-1);
            MYLOG<<"add contact bar to "<<m_listBar->messageWidget->count()-1;
            //面板更新
//            m_mainBar->changeBar("", m_profile->m_contactProfile.last().ip);
//            MYLOG<<"change bar";
        }*/
    });

    connect(m_client, &TcpClient::appendMsg, [=](QString id, message msg){
        int index = m_listBar->messageWidget->currentRow();
        QString c_id = m_profile->m_contact[index];
        m_profile->m_chatList[c_id].append(msg);
        MYLOG<<"current row"<<index;
        MYLOG<<"compare"<<c_id<<" : " <<id;
        if(c_id==id){
            m_mainBar->addMessage(msg);
        }
    });

    MYLOG<<"client initialized!";


    // server启动
    m_server = new TcpServer;
    MYLOG<<QNetworkInterface().allAddresses();
    m_listBar->m_localIpLabel->setText(QNetworkInterface().allAddresses().at(2).toString()); // 存储本地ip
    m_profile->m_ip = QNetworkInterface().allAddresses().at(2).toString(); // 存储本地ip

    connect(m_listBar->portSubmitBtn, &QPushButton::clicked, [&](){
        if(m_listBar->portSubmitBtn->text()=="连接"){
            bool ok = m_server->tcpServer->listen(QHostAddress::Any, m_listBar->m_serverPortEditor->text().toInt());
            if(ok)
            {
                m_listBar->portSubmitBtn->setText("断开");
                MYLOG<<"server listen on"<< m_listBar->m_serverPortEditor->text();
                m_profile->useServer=true;
            }
        }else{
            m_server->closeAll();
            m_listBar->portSubmitBtn->setText("连接");
            MYLOG<<"server closed";
        }

    });

    /*connect(m_server, &TcpServer::addContact, [&](profile pf){
        // 列表增加一项
        m_listBar->addContact(pf);
        m_listBar->messageWidget->setCurrentRow(m_listBar->messageWidget->count()-1);
        //面板更新
//        m_mainBar->changeBar("", pf.ip);
    });*/

    connect(m_profile, &ProfileManager::appendMsg, [=](QString id, message msg){
        int index = m_listBar->messageWidget->currentRow();
        QString c_id = m_profile->m_contact[index];
        m_profile->m_chatList[id].append(msg);
        MYLOG<<"compare"<<c_id<<" : " <<id;
        if(c_id==id){
            m_mainBar->addMessage(msg);
        }
    });

    MYLOG<<"server initialized!";


    // 添加联系人

    connect(m_profile, &ProfileManager::addContact, [=](){
        m_listBar->addContact(m_profile->m_contactProfile[m_profile->m_contact.last()]);
    });

    connect(m_profile, &ProfileManager::updateListBar, [=](){
        m_listBar->update();
    });

    // 发送消息
    connect(m_mainBar->sendBtn, &QPushButton::clicked, [&](){
        int index = m_listBar->messageWidget->currentRow();
        if(index>=0){
            QString id = m_profile->m_contact[index];
            QString remoteIp = m_profile->m_contactProfile[id].ip;

            QString editorMsg = m_mainBar->m_chatEditor->toPlainText();
            m_mainBar->m_chatEditor->setText("");
            QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳
            MYLOG<<"addMessage" << editorMsg << time;
            if(editorMsg != "") {
                message msg;
                msg.id=id;
                msg.ip = m_profile->m_ip;
                msg.msg = editorMsg;
                msg.time = time;
                m_mainBar->addMessage(msg);
                m_profile->m_chatList[id].append(msg);
                QString data = editorMsg;
                if(m_profile->useServer) // 使用服务端发送
                    m_analyzer->sendMessage(m_server->m_tcpClient[remoteIp], "message", &msg);
                else // 使用客户端发送
                    m_analyzer->sendMessage(m_client->m_tcpClient[remoteIp],"message",&msg);
                     // 从client连接池中找到对应ip的客户端并发送消息
            }
        }
    });


    // 更换头像

    connect(m_sideBar->m_avatar,&QtMaterialAvatar::clicked,[=](){
        ChangeHeaderWnd *chooseHeader = new ChangeHeaderWnd();
        chooseHeader->show();
        connect(chooseHeader, &ChangeHeaderWnd::updateHeader,[=](QPixmap header){
            m_sideBar->m_avatar->setImage(header.toImage());
            m_profile->m_avatar = ProfileManager::img2byte(header.toImage());
            chooseHeader->close();
        });
    });

    MYLOG<<"all functions initialized!";

    // SET LAYOUT TO CENTRALWIDGET
    QWidget *mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    MYLOG<<"ui initialezed completed!";
}

void MainWindow::rcvLogin()
{
    setUpUi();
    this->show();
    emit closeLoginWindow();
}


