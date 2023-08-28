#ifndef LEFTBAR_H
#define LEFTBAR_H

#include <QWidget>
#include <QFrame>

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
private:
    QFrame *m_topBar;
    QVBoxLayout *m_layout;
    QtMaterialAvatar *m_avatar;
    QString m_avatarPath = ":/icons/avatar";

};

#endif // LEFTBAR_H
