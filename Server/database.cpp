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

void DataBase::ShowInTable(QTableView* view,QString id,QString name,QString department){
    query.prepare("SELECT CAST(user.id AS NVARCHAR) AS 工号, user.name AS 姓名, department.name AS 部门 FROM user, department WHERE user.department = department.id AND 工号 LIKE ? AND 姓名 LIKE ? AND 部门 LIKE ?");
    query.addBindValue("%" + id + "%");
    query.addBindValue("%" + name + "%");
    query.addBindValue("%" + department + "%");
    if (!query.exec())
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
    cb->addItem("");
    while (query.next()) {
        QString department = query.value(0).toString(); // 假设查询结果在第一列
        cb->addItem(department);
    }
}

int DataBase::isUserAccountCorrect(int id, QString password, QString ip) {//1 means account not found,2 means password incorrect,3 means success
    query.prepare("SELECT * FROM user WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec())
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
    if (query.next()) {
        QString passwordToId = query.value("password").toString();
        if (password != passwordToId)
            return 2;
    } else {
        return 1;
    }
    query.prepare("UPDATE user SET online = 1, ip = ? WHERE id = ?");
    query.addBindValue(ip);
    query.addBindValue(id);
    if (!query.exec())
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
    return 3;
}


void DataBase::setUserAccountOffline(int id){
    query.prepare("UPDATE user SET online = 0 WHERE id = ?");
    QVariantList a;
    a << id;
    query.addBindValue(a);
    if(!query.execBatch())
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
}

QString DataBase::generateRandomPassword(int length) {
    const QString characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()";
    const int charCount = characters.length();
    QString password;
    QRandomGenerator generator = QRandomGenerator::securelySeeded();
    for (int i = 0; i < length; ++i) {
        int randomIndex = generator.bounded(charCount);
        password.append(characters.at(randomIndex));
    }
    return password;
}


QJsonObject DataBase::addUserAccount(int id, QString name, int department) {
    query.prepare("SELECT * FROM user WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec())
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
    if (query.next())
        throw std::runtime_error("Database error: id exists");
    else {
        query.prepare("INSERT INTO user (id, name, department, password) VALUES (?, ?, ?, ?)");
        query.addBindValue(id);
        query.addBindValue(name);
        query.addBindValue(department);
        // Generate and set password here
        QString password = generateRandomPassword(20); // Make sure to implement this method
        query.addBindValue(password);
        if (!query.exec())
            throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
        QJsonObject json;
        json["id"] = id;
        json["name"] = name;
        json["department"] = department;
        json["password"] = password;
        qDebug() << "Succeed to add account!"; // 成功添加账户
        return json;
    }
}


QString DataBase::alterUserPassword(int id, QString passwordOld, QString passwordNew) {
    query.prepare("SELECT * FROM user WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec())
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
    if (!query.next())
        return "The id doesn't exist.";
    else {
        QString passwordToId = query.value("password").toString();
        if (passwordToId != passwordOld)
            return "The old password isn't correct.";
        else {
            query.prepare("UPDATE user SET password = ? WHERE id = ?");
            query.addBindValue(passwordNew);
            query.addBindValue(id);
            if (!query.exec())
                throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
            return "success";
        }
    }
}


QString DataBase::alterUserName(int id, QString name) {
    query.prepare("SELECT * FROM user WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec())
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
    if (!query.next())
        return "The id doesn't exist.";
    else {
        query.prepare("UPDATE user SET name = ? WHERE id = ?");
        query.addBindValue(name);
        query.addBindValue(id);
        if (!query.exec())
            throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
        return "success"; // 成功修改用户名
    }
}

void DataBase::deleteUserAccount(int id) {
    query.prepare("SELECT * FROM user WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec())
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
    if (!query.next())
        throw std::runtime_error("The id doesn't exist.");
    else {
        query.prepare("DELETE FROM user WHERE id = ?");
        query.addBindValue(id);
        if (!query.exec())
            throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
    }
}


bool DataBase::isUserOnline(int id) {
    query.prepare("SELECT online FROM user WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec())
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
    if (query.next()) {
        int online = query.value("online").toInt(); // 转换为整数
        return online;
    }
    throw std::runtime_error("Failed to find user with id: " + QString::number(id).toStdString());
}

//message相关代码
void DataBase::addMessage(QJsonObject jsonMessage){
    int idSender = jsonMessage["sender"].toInt();
    int idReceiver = jsonMessage["receiver"].toInt();
    int timestamp = jsonMessage["timestamp"].toInt();
    QString type = jsonMessage["type"].toString();
    QString messageData = jsonMessage["data"].toString();
    query.prepare("INSERT INTO message(sender, receiver, timestamp, type, data) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(idSender);
    query.addBindValue(idReceiver);
    query.addBindValue(timestamp);
    query.addBindValue(type);
    query.addBindValue(messageData);
    if (!query.exec())
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
}

void DataBase::getMessage(int receiver, QList<QJsonObject> &jsonMessageList){
    query.prepare("SELECT * FROM message WHERE receiver=?");
    query.addBindValue(receiver); // 直接传递整数
    if(!query.exec())
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
    while(query.next()){
        QJsonObject jsonMessage;
        jsonMessage["sender"] = query.value("sender").toInt(); // 转换为整数
        jsonMessage["receiver"] = query.value("receiver").toInt(); // 转换为整数
        jsonMessage["timestamp"] = query.value("timestamp").toInt(); // 转换为整数
        jsonMessage["type"] = query.value("type").toString();
        jsonMessage["data"] = query.value("data").toString();
        jsonMessageList.append(jsonMessage);
    }
}


void DataBase::deleteMessage(int id) {
    query.prepare("DELETE FROM message WHERE receiver = ?");
    query.addBindValue(id);
    if (!query.exec())
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
}


void DataBase::printTableUser(){
    if(!query.exec("SELECT * FROM user"))
        throw std::runtime_error("Database error: " + query.lastError().text().toStdString());
    else{
        while (query.next()) {
            QString id = query.value("id").toString();
            QString name = query.value("name").toString();
            QString password = query.value("password").toString();
            bool online = query.value(3).toInt();
            qDebug() << "id: " << id << " name: " << name << " password: " << password << " online: " << online;
        }
    }
}
