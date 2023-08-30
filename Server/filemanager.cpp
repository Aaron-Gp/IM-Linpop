#include "filemanager.h"

#include <QDir>


FileManager::FileManager(){

}

void FileManager::ToFile(QString binaryData,QString path,QString fileName,int sender,int receiver,int timeStamp){
    QDir dir(path);
    if (!dir.exists())
        if (!dir.mkpath(path))
            throw std::runtime_error ("Failed to create directory:" + path.toStdString());
    QString filePath = path + "/" + fileName+"_"+QString::number(sender) + "_" + QString::number(receiver) + "_" + QString::number(timeStamp);
    QFile outputFile(filePath);
    if (!outputFile.open(QIODevice::WriteOnly))
         throw std::runtime_error ("Failed to open output file:"+filePath.toStdString());
    outputFile.write(binaryData.toUtf8());
    outputFile.close();
}

QString FileManager::ToString(QString path, QString Name,int sender, int receiver, int timeStamp){
    QString fileName = Name+"_"+QString::number(sender) + "_" + QString::number(receiver) + "_" + QString::number(timeStamp);
    QString filePath = path + "/" + fileName;
    QFile inputFile(filePath);
    if (!inputFile.open(QIODevice::ReadOnly))
         throw std::runtime_error("Failed to open input file: " + filePath.toStdString());
    QByteArray binaryData = inputFile.readAll();
    inputFile.close();
    QString stringData = QString::fromUtf8(binaryData); // 将二进制数据转换为 QString
    return stringData;
}

void FileManager::deleteFile(QString path, QString Name, int sender, int receiver, int timeStamp){
     QString fileName = Name + "_" + QString::number(sender) + "_" + QString::number(receiver) + "_" + QString::number(timeStamp);
     QString filePath = path + "/" + fileName;
     if (!QFile::remove(filePath))
         throw std::runtime_error("file open error:"+ filePath.toStdString());
}
