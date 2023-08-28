#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QMap>
#include <QVector>

struct message{
    QString msg;
    QString time;
    QString id;
};

using Dict = QMap<QString, QString>;
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

    QVector<Dict> m_contact;
    QMap<QString,Message> m_chatList;

private:
    ProfileManager(QObject *parent = nullptr);
    static ProfileManager *self;

signals:

};

#endif // PROFILEMANAGER_H
