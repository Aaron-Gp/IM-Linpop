#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QMap>
#include <QVector>

class ProfileManager : public QObject
{
    Q_OBJECT
public:
    static ProfileManager* getInstance();
    QString m_ip;
    QString m_name;
    QString m_id;
    QString m_avatar=":/icons/avatar";

    struct message{
        QString msg;
        QString time;
        QString id;
    };
    QVector<QMap<QString, QString>> m_contact;
    QMap<QString,QVector<message>> m_chatList;

private:
    ProfileManager(QObject *parent = nullptr);
    static ProfileManager *self;

signals:

};

#endif // PROFILEMANAGER_H
