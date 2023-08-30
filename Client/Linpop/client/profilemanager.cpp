#include "profilemanager.h"

#include <QMutex>
#include <QDebug>

ProfileManager* ProfileManager::self = NULL;

ProfileManager *ProfileManager::getInstance()
{
    if (self == NULL) {

        //防止多进程同时调用创建2个对象的问题
        //原理是因为是静态变量只会定义一次
        static QMutex mutex;

        //上锁  QMutexLocker会锁住mutex，当QMutexLocker 被释放的时候会自动解锁
        //locker是局部变量，所以getInstance（）函数结束之后会自动解锁
        QMutexLocker locker(&mutex);

        //在判断一次,防止其它进程抢先new过了
        if (self == NULL) {
            self = new ProfileManager;
        }
    }

    return self; //返回指针
}

void ProfileManager::update(QJsonObject json) {
    QString targetIp = json["ip"].toString(); // 从 JSON 中获取目标 IP
    for (int i = 0; i < m_contactProfile.size(); ++i) {
        if (m_contactProfile[i].ip.contains(targetIp)) {
            m_contactProfile[i].id = QString::number(json["id"].toInt());
            m_contactProfile[i].name = json["name"].toString();
        }
    }
}


ProfileManager::ProfileManager(QObject *parent) : QObject(parent)
{

}
