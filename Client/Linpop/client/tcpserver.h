#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QMap>
#include <profilemanager.h>
#include "msganalyzer.h"
#include <global.h>

class TcpServer : public QWidget
{
    Q_OBJECT
public:
    explicit TcpServer(QWidget *parent = nullptr);
    ~TcpServer();
    void closeAll();

    ProfileManager *m_profile;
    QTcpServer *tcpServer;
    QMap<QString, QTcpSocket*> m_tcpClient;
    QMap<QString, QTcpSocket*> m_fileClient;
    QTcpSocket *currentClient;
    MsgAnalyzer *m_analyzer;

signals:

private slots:
    void NewConnectionSlot();


};

#endif // TCPSERVER_H
