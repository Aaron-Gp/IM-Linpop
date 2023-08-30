#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client =new TcpClient(this);
    TCPServer server(this);
    connect(this->ui->pushButton, &QPushButton::clicked,this,&MainWindow::slotSendTo);
}

void MainWindow::slotSendTo(){
    QString text = ui->textEdit->toPlainText();
    client->slotSendMsg(text);
}

MainWindow::~MainWindow()
{
    delete ui;
}

