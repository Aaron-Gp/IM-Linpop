#ifndef GLOBAL_H
#define GLOBAL_H

#define SERVER QString("127.0.0.1")
#define PORT 6666
#define DATABASE "../"
#define DATABASENAME "data.db"
#define FILEPATH "../FILE"
#include <QDebug>
#define MYLOG qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"
//#define QT_NO_DEBUG_OUTPUT

#endif // GLOBAL_H
