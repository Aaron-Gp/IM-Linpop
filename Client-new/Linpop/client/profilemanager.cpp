#include "profilemanager.h"
#include <QMutex>
#include <QBuffer>
#include <QPixmap>
#include <QByteArray>
#include "clientdatabase.h"

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

QString ProfileManager::img2byte(const QImage& img)
{
    QByteArray ba;
    QBuffer buf(&ba);
    buf.open(QIODevice::WriteOnly);

    img.save(&buf, "png");
    QByteArray ba2 = ba.toBase64();
    QString b64str = QString::fromUtf8(ba2);

    return b64str;
}

QImage ProfileManager::byte2img(const QString& p_b)
{
    MYLOG<<"Here";
    QImage img;
    QByteArray arr_base64 = p_b.toUtf8();
    img.loadFromData(QByteArray::fromBase64(arr_base64));

    return img;
}

void ProfileManager::updateMessage()
{
    m_chatList=m_db->getMessage();
    emit update();
}


void ProfileManager::updateProfiles(){
    m_contactProfile=m_db->getProfile();
    m_contact.clear();
    for (const QString &id : m_contactProfile.keys()) {
        m_contact.append(id);
    }
    emit updateListBar();
}



ProfileManager::ProfileManager(QObject *parent) : QObject(parent)
{
    m_db = new ClientDataBase;
    m_avatar = img2byte(QImage(":/icons/avatar"));
}
