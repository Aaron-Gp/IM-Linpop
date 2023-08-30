#include <QVBoxLayout>
#include <qtmaterialavatar.h>
#include "sidebar.h"
#include <QToolButton>

SideBar::SideBar(QWidget *parent) : QFrame(parent)
{
    m_profile = ProfileManager::getInstance();
    setStyleSheet("QFrame{"
                  "background:rgb(234,234,234);"
                  "}"
                  "QFrame QToolButton{"
                  "border: 0;"
                  "}"
                  "QFrame QToolButton:hover{"
                  "background:rgb(205,205,205);"
                  "border-radius: 10px;"
                  "border: 1px solid rgb(234,234,234);"
                  "}");
    setFrameStyle(QFrame::Box);
    setLineWidth(0);
    setFixedWidth(72);
    m_layout = new QVBoxLayout(this);
    m_layout->setAlignment(Qt::AlignTop);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);
    setupUi();
}

void SideBar::setupUi()
{
    setupTopBar();
    setupToolBar();
}

void SideBar::setupTopBar()
{
    m_avatar = new QtMaterialAvatar(m_profile->byte2img(m_profile->m_avatar));
    m_topBar = new QFrame();
    m_topBar->setFrameStyle(QFrame::Box);
    m_topBar->setLineWidth(0);
    m_topBar->setFixedHeight(54);
    QHBoxLayout *avatarTLayout = new QHBoxLayout();
    avatarTLayout->setContentsMargins(0,0,0,0);
    avatarTLayout->setSpacing(0);
    avatarTLayout->addWidget(m_avatar);
    m_topBar->setLayout(avatarTLayout);
    m_layout->addWidget(m_topBar);
}

void SideBar::setupToolBar()
{
    QVBoxLayout *actionTLayout = new QVBoxLayout();
    actionTLayout->setContentsMargins(0,12,0,12);
    actionTLayout->setSpacing(12);
    actionTLayout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

    QToolButton *messageBtn = new QToolButton();
    messageBtn->setIcon(QIcon(":/icons/chat"));
    messageBtn->setFixedSize(QSize(48,48));
    messageBtn->setIconSize(QSize(32,32));
    toolVector.append(messageBtn);
    actionTLayout->addWidget(messageBtn);
    QToolButton *contactBtn = new QToolButton();
    contactBtn->setIcon(QIcon(":/icons/person"));
    contactBtn->setFixedSize(QSize(48,48));
    contactBtn->setIconSize(QSize(32,32));
    toolVector.append(contactBtn);
    actionTLayout->addWidget(contactBtn);
    QToolButton *p2pBtn = new QToolButton();
    p2pBtn->setIcon(QIcon(":/icons/p2p"));
    p2pBtn->setFixedSize(QSize(48,48));
    p2pBtn->setIconSize(QSize(32,32));
    toolVector.append(p2pBtn);
    actionTLayout->addWidget(p2pBtn);
    actionTLayout->addStretch();
    QToolButton *settingBtn = new QToolButton();
    settingBtn->setIcon(QIcon(":/icons/setting"));
    settingBtn->setFixedSize(QSize(48,48));
    settingBtn->setIconSize(QSize(32,32));
    actionTLayout->addWidget(settingBtn);


    m_layout->addLayout(actionTLayout);
}
