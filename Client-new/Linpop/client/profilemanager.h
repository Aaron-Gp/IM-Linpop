#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QList>
#include <QImage>
#include "global.h"

class ClientToServer;
class ClientDataBase;

typedef struct{
    QString msg;
    QString time;
    QString ip;
    QString id;
    QString isSender;
}message;

typedef struct{
    QString id;
    QString ip;
    QString avatar;
    QString name;
}profile;

using Message = QVector<message>;

class ProfileManager : public QObject
{
    Q_OBJECT
public:
    static ProfileManager* getInstance();
    static QString img2byte(const QImage& img);
    static QImage byte2img(const QString& p_b);

    QString m_ip;
    QString m_name;
    QString m_id;
    QString m_password;
    QString m_avatar; // 存base64编码
    bool useServer = false;

    QList<QString> m_contact;
    QMap<QString,profile> m_contactProfile;
    QMap<QString,Message> m_chatList;
    ClientToServer *m_clientToServer;
    ClientDataBase *m_db;

public slots:
    void updateMessage();
    void updateProfiles();
private:
    ProfileManager(QObject *parent = nullptr);
    static ProfileManager *self;

signals:
    void addContact();
    void appendMsg(QString id, message msg);
    void updateListBar();
    void update();
};

#endif // PROFILEMANAGER_H
