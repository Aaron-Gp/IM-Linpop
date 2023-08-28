#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Definition.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    db=new DataBase(PATH,this);
    ui->setupUi(this);
    QAction *actionUsersList = new QAction("用户列表", this);
    ui->toolBar->addAction(actionUsersList);
    QAction *actionAddUser = new QAction("新增用户", this);
    ui->toolBar->addAction(actionAddUser);

    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(actionUsersList, &QAction::triggered, signalMapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
    connect(actionAddUser, &QAction::triggered, signalMapper, static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
    // 设置映射规则
    signalMapper->setMapping(actionUsersList, 1); // 0 表示页面索引
    signalMapper->setMapping(actionAddUser, 0); // 1 表示页面索引

    // 连接 QSignalMapper 的 mapped 信号到槽函数
    connect(signalMapper, static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped), this, &MainWindow::slotSwitchToPage);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::slotDeleteFromDataBase);
    //db->ShowInTable(ui->tableView,"SELECT * FROM USER");

    db->ShowInTable(ui->tableView, "SELECT user.user_id AS 工号, user.name AS 姓名, department.name AS 部门 FROM user, department WHERE user.department = department.id");
    db->ShowInComboBox(ui->comboBox,"SELECT NAME FROM DEPARTMENT");
    connect(ui->pushButton_2,&QPushButton::clicked, this, &MainWindow::slotSearch);

    server = new TcpServer(this);

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::slotSwitchToPage(int i){
    ui->stackedWidget->setCurrentIndex(i);
}

void MainWindow::slotDeleteFromDataBase(){
    try{
        deleteFromDataBase(db,"User");
    }catch(const std::exception &e){
        throw e;
    }
}

void MainWindow::slotSearch(){
    QString id=ui->lineEdit->text();
    QString name=ui->lineEdit_2->text();
    QString department=ui->comboBox->currentText();
    db->ShowInTable(ui->tableView, QString("SELECT * FROM (SELECT CAST(user.user_id AS NVARCHAR) AS 工号, user.name AS 姓名, department.name AS 部门 FROM user, department WHERE user.department = department.id) where 工号 like '%%1%' and 姓名 like '%%2%' and 部门 like '%%3%'").arg(id).arg(name).arg(department));
}

void MainWindow::deleteFromDataBase(DataBase* db, QString tableName){
    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
    if (!selectedIndexes.isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete this user?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            int selectedRow = selectedIndexes.first().row();
            // 获取行中的主键或标识，根据需要进行数据库删除操作
            QAbstractItemModel *model = ui->tableView->model();// 假设第一列是主键
            QVariant data = model->data(model->index(selectedRow, 0));
            QString value=data.toString();
            //QString key = ui->tableView->horizontalHeader()->model()->headerData(0, Qt::Horizontal).toString();
            QString key="user_id";
            try{
                //qDebug()<<QString("DELETE FROM %1 WHERE %2 = %3").arg(tableName).arg(key).arg(value);
                db->exec(QString("DELETE FROM %1 WHERE %2 = %3").arg(tableName).arg(key).arg(value));
                //qDebug() << "Row deleted successfully";
                db->ShowInTable(ui->tableView, QString("SELECT * FROM %1").arg(tableName));
            }catch(const std::exception &e){
                throw e;
            }
        }
    }else
        QMessageBox::information(this, "Message", "No row selected.");
}
