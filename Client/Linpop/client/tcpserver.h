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
    void appendMsg(QString ip, message msg);
    void addContact(profile pf);


private slots:
    void NewConnectionSlot();
//    void disconnectedSlot();
//    void ReadData();

//    void on_btnConnect_clicked();
//    void on_btnSend_clicked();
//    void on_btnClear_clicked();

};

#endif // TCPSERVER_H
