#include "profilemanager.h"
#include <QMutex>

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

ProfileManager::ProfileManager(QObject *parent) : QObject(parent)
{

}
