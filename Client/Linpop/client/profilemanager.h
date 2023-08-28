#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QMap>
#include <QVector>

class ProfileManager : public QObject
{
    Q_OBJECT
public:
    explicit ProfileManager(QObject *parent = nullptr);



    QString m_ip;
    QString m_name;
    QString m_id;
    QString m_avatar;

    struct message{
        QString msg;
        QString time;
        QString id;
    };

    QVector<QMap<QString, QString>> m_contact;
    QMap<QString,QVector<message>> m_chatList;

signals:

};

#endif // PROFILEMANAGER_H
