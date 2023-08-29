#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QList>
#include "clientdatabase.h"
#include "global.h"

class ClientToServer;

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
    QString avatar=":/icons/person";
    QString name;
}profile;

using Message = QVector<message>;

class ProfileManager : public QObject
{
    Q_OBJECT
public:
    static ProfileManager* getInstance();
    QString img2byte(QImage img);
    QPixmap byte2img(QString p_b);

    QString m_ip;
    QString m_name;
    QString m_id;
    QString m_password;
    QString m_avatar=":/icons/avatar";
    bool useServer = false;

    QList<QString> m_contact;
    QVector<profile> m_contactProfile;
    QMap<QString,Message> m_chatList;
    ClientToServer *m_clientToServer;
    ClientDataBase *m_db;

private:
    ProfileManager(QObject *parent = nullptr);
    static ProfileManager *self;

signals:

};

#endif // PROFILEMANAGER_H
