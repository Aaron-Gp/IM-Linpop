#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
#include <profilemanager.h>

class TcpServer : public QWidget
{
    Q_OBJECT
public:
    explicit TcpServer(QWidget *parent = nullptr);
    ~TcpServer();
    void closeAll();

    ProfileManager *m_profile;
    QTcpServer *tcpServer;
    QList<QTcpSocket*> tcpClient;
    QTcpSocket *currentClient;


private slots:
    void NewConnectionSlot();
    void disconnectedSlot();
    void ReadData();

//    void on_btnConnect_clicked();
//    void on_btnSend_clicked();
//    void on_btnClear_clicked();

};

#endif // TCPSERVER_H
