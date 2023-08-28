#ifndef MSG_H
#define MSG_H

#include <QString>
#include <QObject>
#include <QMetaType>
#include <QTcpSocket>

class Msg{
public:
    QString buf;
    QString ip;
    int port;
    QTcpSocket* socket;
    Msg() = default;
    Msg(QTcpSocket* socket,int port,QString ip,QString buf);
};

Q_DECLARE_METATYPE(Msg);

#endif // MSG_H
