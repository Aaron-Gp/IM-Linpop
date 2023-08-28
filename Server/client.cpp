#include "client.h"

Client::Client(int port, QString ip, QTcpSocket *sock)
{
    this->port=port;
    this->ip=ip;
    this->sock=sock;
}
