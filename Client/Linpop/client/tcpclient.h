#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QAbstractSocket>
#include <profilemanager.h>
#include "msganalyzer.h"
#include "global.h"

class TcpClient : public QWidget
{
    Q_OBJECT
public:
    explicit TcpClient(QWidget *parent = nullptr);
    bool newConnection(QString ip);
    void sendData(QString ip, QString msg);
    ~TcpClient();
    ProfileManager *m_profile;
    QMap<QString, QTcpSocket*> m_tcpClient;
    QMap<QString, QTcpSocket*> m_fileClient;
    MsgAnalyzer *m_analyzer;

signals:


};

#endif // TCPCLIENT_H
