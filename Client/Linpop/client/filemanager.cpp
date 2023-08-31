#include "filemanager.h"

#include <QDir>
#include "global.h"

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

QString FileManager::ToString(QString filePath){
    QFile inputFile(filePath);
    if (!inputFile.open(QIODevice::ReadOnly))
        throw std::runtime_error("Failed to open input file: " + filePath.toStdString());
    QByteArray binaryData = inputFile.readAll();
    inputFile.close();
    QString stringData = QString::fromUtf8(binaryData); // 将二进制数据转换为 QString
    return stringData;
}

QString FileManager::ToFile(QString fileName, QString id,bool isSender,QString binaryData){
    QString path=QString(FILEPATH)+ QDir::separator() +id+ QDir::separator() +(isSender?"sender":"receiver");
    QDir dir(path);
    if (!dir.exists())
        if (!dir.mkpath(path))
            throw std::runtime_error ("Failed to create directory:" + QString(path).toStdString());
    QString fullPath = QString(path) + QDir::separator() + fileName;
    QFileInfo fileInfo(fullPath);
    QString baseName = fileInfo.baseName();
    QString completeSuffix = fileInfo.completeSuffix();
    QString newBaseName = baseName;
    int suffixIndex = 1;
    // 检查文件是否已经存在，如果存在就添加后缀
    while (fileInfo.exists()) {
        newBaseName = QString("%1_%2").arg(baseName).arg(suffixIndex);
        fullPath = QString(path) + QDir::separator() + newBaseName + "." + completeSuffix;
        fileInfo.setFile(fullPath);
        suffixIndex++;
    }
    // 将 binaryData 转换为 QByteArray
    QByteArray binaryArray = binaryData.toUtf8(); // 使用 toUtf8 或 toLatin1，取决于二进制数据的编码
    // 创建 QFile 对象并打开文件
    QFile file(fullPath);
    if (file.open(QIODevice::WriteOnly)) {
        // 使用 QTextStream 写入二进制数据
        QTextStream out(&file);
        out << binaryArray;
        file.close();
        qDebug() << "File saved.";
    } else {
        qDebug() << "Failed to save file.";
    }
    return fullPath;
}

void FileManager::deleteFile(QString path, QString Name, int sender, int receiver, int timeStamp){
    QString fileName = Name + "_" + QString::number(sender) + "_" + QString::number(receiver) + "_" + QString::number(timeStamp);
    QString filePath = path + "/" + fileName;
    if (!QFile::remove(filePath))
        throw std::runtime_error("file open error:"+ filePath.toStdString());
}

