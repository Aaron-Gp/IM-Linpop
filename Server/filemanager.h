#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QFile>
#include <QDir>

class FileManager
{
public:
    FileManager();
    static void ToFlie(QString binaryData,QString path,QString fileName,int sender,int receiver,int timestamp);
    static QString ToString(QString path, QString Name,int sender, int receiver, int timeStamp);
    static void deleteFile(QString path, QString Name,int sender, int receiver, int timeStamp);
};

#endif // FILEMANAGER_H
