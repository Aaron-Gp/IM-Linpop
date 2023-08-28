#include "tcpserver.h"

TcpServer::TcpServer(QObject* parent){
    //m_client=NULL;
    m_server = new QTcpServer(this);
    //将套接字设置为监听模式
    m_server->listen(QHostAddress::Any, PORT);
    //通过信号接收客户端请求
    connect(m_server, &QTcpServer::newConnection,
            this, &TcpServer::slotNewConnection);//once the server connected, anaylze the client
    m_clients=new QList<Client>;
}

void TcpServer::slotNewConnection(){
    QTcpSocket* curSocket = m_server->nextPendingConnection();
    //! 获取远端客户端ip和端口号
    QString ip = curSocket->peerAddress().toString();
    int port = curSocket->peerPort();
    emit signal_newClient(ip, port);
    m_clients->append(Client(port,ip,curSocket));
    //连接
    connect(curSocket, &QTcpSocket::readyRead, this, &TcpServer::slotReceiveData);
    connect(curSocket, &QTcpSocket::disconnected, this, &TcpServer::slotDisconnect);
}

void TcpServer::handleNewMessage(QTcpSocket* socket,Msg msg){
    connect(socket, &QTcpSocket::readyRead, this, &TcpServer::slotReceiveData);
    MsgAnalyzer* analyzer=new MsgAnalyzer(msg,m_clients);
    QThread* newThread=new QThread;
    QObject::connect(newThread, &QThread::started, analyzer, &MsgAnalyzer::anaylze);
    QObject::connect(analyzer, &MsgAnalyzer::finish, analyzer, &MsgAnalyzer::deleteLater);
    QObject::connect(analyzer, &MsgAnalyzer::send,this,&TcpServer::SendMsgTo);
    QObject::connect(analyzer, &MsgAnalyzer::finish, newThread, [newThread]() {
        newThread->quit();
        newThread->wait(); // Wait for the thread to finish before cleaning up
        newThread->deleteLater();
    });
    analyzer->moveToThread(newThread);
    newThread->start();
}


void TcpServer::reconnect(QTcpSocket* socket){
    connect(socket, &QTcpSocket::readyRead, this, &TcpServer::slotReceiveData);
}

void TcpServer::slotReceiveData(){//接收数据
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    disconnect(socket, &QTcpSocket::readyRead, this, &TcpServer::slotReceiveData);
    SocketTask* task= new SocketTask(socket,this);
    QThreadPool::globalInstance()->start(task);
}

void TcpServer::slotDisconnect(){
    for (QList<Client>::iterator iter = m_clients->begin(); iter != m_clients->end(); iter++){
        if (QAbstractSocket::UnconnectedState == iter->sock->state()){
            iter = m_clients->erase(iter, iter + 1);
            iter--;
            emit signal_disConncet(iter->ip, iter->port);
        }
    }
}

void TcpServer::SendMsg(Msg msg){
    for (QList<Client>::iterator iter = m_clients->begin(); iter != m_clients->end(); iter++){
        if (iter->ip == msg.ip && iter->port == msg.port) {
            // 使用客户端的 socket 发送消息
            if (iter->sock->write(msg.buf.toStdString().c_str()) != -1)
                qDebug() << "Message sent successfully.";
            else
                qDebug() << "Message sending failed.";
            return;
        }
    }
}

void TcpServer::SendMsgTo(QTcpSocket* socket,Msg msg){
    socket->write(msg.buf.toStdString().c_str());
}
