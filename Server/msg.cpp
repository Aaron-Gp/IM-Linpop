#include "msg.h"

Msg::Msg(QTcpSocket* socket,int port,QString ip,QString buf){
    this->socket = socket;
    this->buf = buf;
    this->ip = ip;
    this->port = port;
}

