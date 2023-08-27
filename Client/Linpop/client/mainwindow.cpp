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
    resize(800,600);
    setFixedSize(800,600);
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
    actionTLayout->addWidget(messageBtn);
    QToolButton *contactBtn = new QToolButton();
    contactBtn->setIcon(QIcon(":/icons/person"));
    contactBtn->setFixedSize(QSize(48,48));
    contactBtn->setIconSize(QSize(32,32));
    actionTLayout->addWidget(contactBtn);
    actionTLayout->addStretch();
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
    QFrame *listFrame = new QFrame();
    listFrame->setFrameStyle(QFrame::Box);
    listFrame->setLineWidth(1);
    listFrame->setFixedWidth(150);
    QVBoxLayout *listLayout = new QVBoxLayout();
    listLayout->setContentsMargins(0,0,0,0);
    listLayout->setSpacing(0);

    // ACTIONFRAME
    QFrame *actionLFrame = new QFrame();
    actionLFrame->setFrameStyle(QFrame::Box);
    actionLFrame->setLineWidth(1);
    actionLFrame->setFixedHeight(54);
    QHBoxLayout *actionLLayout = new QHBoxLayout();
    actionLLayout->setContentsMargins(8,0,8,0);
    actionLLayout->setSpacing(4);
    actionLLayout->setAlignment(Qt::AlignLeft);

    QLineEdit *searchBar = new QLineEdit();
    searchBar->setPlaceholderText("搜索");
    searchBar->setFixedHeight(32);
    actionLLayout->addWidget(searchBar);
    QToolButton *addContactBtn = new QToolButton();
    addContactBtn->setIcon(QIcon(":/icons/add-person"));
    addContactBtn->setFixedSize(QSize(32,32));
    addContactBtn->setIconSize(QSize(16,16));
    actionLLayout->addWidget(addContactBtn);

    // END ACTIONLFRAME
    actionLFrame->setLayout(actionLLayout);
    listLayout->addWidget(actionLFrame);

    // LISTFRAME
    QFrame *listLFrame = new QFrame();
    listLFrame->setFrameStyle(QFrame::Box);
    listLFrame->setLineWidth(1);


    // END LISTFRAME
    listLayout->addWidget(listLFrame);

    // END LISTFRAME in MIDDLE
    listFrame->setLayout(listLayout);
    return listFrame;
}

QFrame *MainWindow::rightUi()
{
    // MAINFRAME in RIGHT
    QFrame *mainFrame = new QFrame();
    mainFrame->setFrameStyle(QFrame::Box);
    mainFrame->setLineWidth(1);
    QSizePolicy mFSizePolicy = mainFrame->sizePolicy();
    mFSizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    mainFrame->setSizePolicy(mFSizePolicy);
    return mainFrame;
}

void MainWindow::makeFakeData()
{
    fakeListData = new QJsonArray();
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
