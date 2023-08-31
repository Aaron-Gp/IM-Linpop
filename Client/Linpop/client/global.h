#ifndef GLOBAL_H
#define GLOBAL_H

#define SERVER QString("192.168.163.130")
#define PORT 6666
#define DATABASE QString("./data")
#define FILEPATH QString("../FILE")

#include <QDebug>
#define MYLOG qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"
//#define QT_NO_DEBUG_OUTPUT

#endif // GLOBAL_H
