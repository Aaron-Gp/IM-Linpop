#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QFile>
#include <QDir>

class FileManager
{
public:
    FileManager();
    static void ToFile(QString binaryData,QString path,QString fileName,int sender,int receiver,int timestamp);
    static QString ToString(QString path, QString Name,int sender, int receiver, int timeStamp);
    static void deleteFile(QString path, QString Name,int sender, int receiver, int timeStamp);
    static QString ToString(QString filePath);
    static QString ToFile(QString fileName,QString id,bool isSender,QString binaryData);
};

#endif // FILEMANAGER_H
