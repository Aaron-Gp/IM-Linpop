#ifndef SOCKETTASK_H
#define SOCKETTASK_H

#include <QRunnable>
#include <QTcpSocket>
#include "msg.h"
#include <QHostAddress>
#include <QElapsedTimer>
#include <QObject>
#include <QThread>

class SocketTask : public QRunnable{
public:
    explicit SocketTask(QTcpSocket *socket,QObject* m_pObj);
    void run() override;
private:
    QTcpSocket *socket;
    QObject * m_pObj = nullptr;
};
#endif // SOCKETTASK_H
