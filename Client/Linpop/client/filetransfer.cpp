//#include "filetransfer.h"
//#include<QDataStream>
//#include<QMessageBox>
//#include<QDateTime>
//#include<QFileDialog>
//#include<QDebug>
//#include <QJsonArray>
//#include <QJsonObject>
//#include <QJsonDocument>
//#include <QTcpSocket>
//#include <QHostAddress>
//#include<QTcpServer>
//#include <QElapsedTimer>
//#include <QThread>
//FileTransfer::FileTransfer(QWidget *parent) : QWidget(parent)
//{
//    isReadFile=true;
//    /*
//    QPushButton *fileSendBtn = new QPushButton("发送文件");
//    connect(fileSendBtn,&QPushButton::clicked,this,[=](){
//           sendMsg(FileMsg);
//       });
//    sendCLayout->addWidget(fileSendBtn);


//        connect(fileBtn,&QPushButton::clicked,this,[=](){
//               fileChoose();
//           });
//    */
//}

//void FileTransfer::sendMsg(FileTransfer::msgType type)
//{
//    QJsonObject messageObj; // Create a JSON object for the message
//    QString str=MainWindow::chatEditor->toPlainText();
//    chatEditor->setText("");
//    switch (type)
//    {
//    case Msg:
//        messageObj["function"] = "information";
//        messageObj["length"] = str.length();
//        messageObj["type"] = "text";
//        messageObj["sender"] = 1;
//        messageObj["receiver"] = 2; // Fill in the appropriate receiver
//        messageObj["data"] = str;

//        break;
//    case FileMsg:
//        messageObj["function"] = "information";
//        messageObj["type"] = "file";
//        messageObj["sender"] = "";
//        messageObj["receiver"] = ""; // Fill in the appropriate receiver
//        messageObj["fileName"] = fileName;
//        messageObj["size"] = fileSize;
//        break;
//    }

//    if(tcpSocket->state() == QAbstractSocket::ConnectedState){
//        QJsonDocument messageDoc(messageObj);
//        QByteArray dataArray = messageDoc.toJson(QJsonDocument::Compact);
//        tcpSocket->write("<?BEGIN?>");
//        tcpSocket->write(dataArray);
//        tcpSocket->write("<?END?>");
//        if(type==FileMsg){
//            qint64 len = 0;for(int i=0;i<10000000;i++);
//                    do {
//                        // 每次发送数据的大小
//                        char buf[4 * 1024] = { 0 };
//                        len = 0;
//                        // 往文件中读数据
//                        len = file.read(buf, sizeof(buf));
//                        // 发送数据，读多少，发多少
//                        len = tcpSocket->write(buf, len);
//                        // 发送的数据需要累计
//                        sendSize += len;
//                    } while (len > 0);
//                    // 是否发送文件完毕
//                    if (sendSize == fileSize) {
//                        QMessageBox::information(this, "提示", QString("发送文件完毕！ 文件大小：%1").arg(sendSize));
//                        file.close();
//                        // 把客户端断开
//                        //tcpSocket->disconnectFromHost();
//                        //tcpSocket->close();
//                    }
//        }
//    }else{
//        qDebug() << "Socket is not connected yet. Cannot send message.";
//    }

//}


//void TCPClient::fileChoose()
//{
//    QString filePath = QFileDialog::getOpenFileName(this, "open", "../");
//        if (filePath.isEmpty() == false) {
//            fileName.clear();
//            fileSize = 0;
//            // 获取文件信息
//            QFileInfo info(filePath);
//            fileName = info.fileName();
//            fileSize = info.size();

//            sendSize = 0;	// 发送文件的大小

//            // 只读方式打开文件
//            // 指定文件的名字
//            file.setFileName(filePath);

//            // 打开文件
//            bool isOK = file.open(QIODevice::ReadOnly);
//            if (isOK == false) {
//                QMessageBox::information(this, "提示", "只读方式打开文件失败！");
//                return;
//            }

//            // 提示打开文件的路径
//           // ui->FileEdit->setText(filePath);

//            // 发送按钮放开
//            //ui->fileSendBtn->setEnabled(true);
//        } else {
//            QMessageBox::information(this, "提示", "选择文件路径出错！");
//        }
//}

//#define BEGIN "<?BEGIN?>"
//#define END "<?END?>"
//#define Timeout 1000
//void TCPClient::ReceiveMessage()
//{
//    if (tcpSocket->isReadable()) {
//        QElapsedTimer timer;
//        timer.start();
//        QByteArray receivedData;
//        bool foundBegin = false;
//        while (true) {
//            QByteArray buf = tcpSocket->read(1024);
//            if(buf.length()!=0)
//                timer.restart();
//            else
//                QThread::msleep(50);
//            if(timer.hasExpired(Timeout)){

//                return;
//            }
//            receivedData.append(buf);
//            if (!foundBegin) {
//                int beginIndex = receivedData.indexOf(BEGIN);
//                if (beginIndex != -1) {
//                    receivedData.remove(0, beginIndex + QString(BEGIN).length());
//                    foundBegin = true;
//                }else{
//                    //throw std::runtime_error("invalid information");
//                    qDebug() << "invalid message:" << receivedData;

//                    return;
//                }
//            }
//            if (foundBegin && receivedData.contains(END)) {
//                int endIndex = receivedData.indexOf(END);
//                QByteArray message = receivedData.mid(0, endIndex);
//                processMessage(message);
//                qDebug() << "Received message:" << message;
//                receivedData.remove(0, endIndex + strlen(END));
//                foundBegin = false; // Reset for the next message
//                if (receivedData.length()==0)
//                    break;
//            }
//        }
//    }
//}
//void TCPClient::processMessage(QByteArray message){
//    QJsonDocument messageDoc = QJsonDocument::fromJson(message);
//    QJsonObject messageObj = messageDoc.object();

//    QString function = messageObj["function"].toString();
//    if (function == "information")
//    {
//        QString type = messageObj["type"].toString();
//        //QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//        if (type == "text")
//        {
//            QString sender = messageObj["sender"].toString();
//            QString data = messageObj["data"].toString();
//            QString time = messageObj["timeStamp"].toString();
//            chatBroswer->addItem("[" + sender + "]" + time);
//            chatBroswer->addItem(data);

//            //ClientDataBase::Message(messageObj);
//        }
//        else if (type == "fileMsg"){
//            isReadFile=false;
//            QString sender = messageObj["sender"].toString();
//            recvFileName = messageObj["fileName"].toString();
//            recvSize = messageObj["fileSize"].toInt();
//            //QByteArray data = messageObj["data"].toString().toUtf8();
//            recvFile.setFileName(recvFileName);

//            bool isOk = recvFile.open(QIODevice::WriteOnly);
//            if (isOk == false) {
//                QMessageBox::information(this, "提示", "打开文件失败！");
//                return;
//            }
//        }
//    }
//}
