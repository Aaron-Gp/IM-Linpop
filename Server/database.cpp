#include "database.h"

int DataBase::count = 0;

DataBase::~DataBase(){
    database.close();
}

DataBase::DataBase(QString path,QObject* parent){
    database = QSqlDatabase::addDatabase("QSQLITE",QString("connection_%1").arg(count++));
    database.setDatabaseName(path); // 替换为你的数据库文件路径
    if (!database.open()) {
        throw std::runtime_error("Failed to connect to database!");
    }
    query = QSqlQuery(database);
}

void DataBase::exec(QString sql){
    bool success = query.exec(sql);
    if(!success)
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
}

void DataBase::close(){
    database.close();
}

void DataBase::ShowInTable(QTableView* view, QString sql) {
    bool success = query.exec(sql); // 执行查询
    if(!success)
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
    QSqlQueryModel* model = new QSqlQueryModel; // 创建查询模型
    model->setQuery(query); // 将查询结果设置到模型中
    // 设置代理模型以实现排序
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel();
    proxyModel->setSourceModel(model);
    view->setModel(proxyModel);
    view->horizontalHeader()->
        setSectionResizeMode(QHeaderView::Stretch);//填充整个窗口
    view->setSelectionBehavior(QAbstractItemView::SelectRows);//一次选中一行
    view->setSortingEnabled(true);
    view->verticalHeader()->hide();
}

void DataBase::ShowInComboBox(QComboBox* cb, QString sql){
    // 执行查询
    bool success = query.exec(sql); // 执行查询
    if(!success)
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
    // 从查询结果中提取数据并添加到 QComboBox
    cb->clear(); // 清空之前的内容
    while (query.next()) {
        QString department = query.value(0).toString(); // 假设查询结果在第一列
        cb->addItem(department);
    }
}
