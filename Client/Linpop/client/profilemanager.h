#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QList>
#include <QTcpSocket>
#include "global.h"

typedef struct{
    QString msg;
    QString time;
    QString ip;
}message;

typedef struct{
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
    QString m_ip="127.0.0.1";
    QString m_name;
    QString m_id;
    QString m_avatar=":/icons/avatar";
    bool useServer = false;

    QList<QString> m_contact;
    QVector<profile> m_contactProfile;
    QMap<QString,Message> m_chatList;
    QTcpSocket *m_clientToServer;

private:
    ProfileManager(QObject *parent = nullptr);
    static ProfileManager *self;

signals:

};

#endif // PROFILEMANAGER_H
