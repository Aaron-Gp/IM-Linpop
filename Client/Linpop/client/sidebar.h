#ifndef LEFTBAR_H
#define LEFTBAR_H

#include <QWidget>
#include <QFrame>
#include "global.h"
#include "profilemanager.h"

class QVBoxLayout;
class QtMaterialAvatar;
class QToolButton;

class SideBar : public QFrame
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);
    void setAvatar(QString path);
private:
    void setupUi();
    void setupTopBar();
    void setupToolBar();

signals:

public:
    QVector<QToolButton*> toolVector;
    QFrame *m_topBar;
    QVBoxLayout *m_layout;
    QtMaterialAvatar *m_avatar;
    QString m_avatarPath = ":/icons/avatar";
    ProfileManager *m_profile;

};

#endif // LEFTBAR_H
