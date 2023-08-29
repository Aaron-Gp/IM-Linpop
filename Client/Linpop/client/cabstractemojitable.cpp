#include "cabstractemojitable.h"
#include<QHeaderView>
#include<QLabel>
#include<QDebug>

CAbstractEmojiTable::CAbstractEmojiTable(QWidget *parent)
    : QTableWidget(parent)
{
//    setRowCountAndColumnCount(56, 10);
    setMouseTracking(true);
    addEmoji();
    initStyle();
    initConnect();
}


CAbstractEmojiTable::~CAbstractEmojiTable()
{
    clearContents();
}

void CAbstractEmojiTable::addEmoji()
{
    if (rowCount() == 0 || columnCount() == 0) {
        return;
    }
    int columncount = columnCount();
    int emojiIndex = 0;
    int rowIndex = 0;
    int columnIndex = 0;
    //遍历map
    //code2url是一个存放表情编码和对应的图片url的QMap，表情编码是预先和同事协商的
    //for example: [happy]:":/emoji/happy.png"
    auto it = code2url.begin();
    while (it != code2url.end()) {
        rowIndex = emojiIndex / columncount;
        columnIndex = emojiIndex % columncount;
        QLabel * label = new QLabel(this);
        label->setPixmap(QPixmap(it.value()));
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);
        label->setToolTip(it.key());
        setCellWidget(rowIndex, columnIndex, label);
        ++emojiIndex;
        ++it;
    }
}

void CAbstractEmojiTable::initStyle()
{
    setFixedSize(250,160);
    verticalHeader()->setVisible(false);
    horizontalHeader()->setVisible(false);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setShowGrid(false);
    setObjectName("BaseEmojiWgt");
    setStyleSheet("QLabel:hover{border:1px solid #caced1;}"\
        "QTableWidget#BaseEmojiWgt{border:1px solid #D8DCE0;}"\
        "QTableWidget::item:selected{background-color:white;outline:0px;}");
}


void CAbstractEmojiTable::initConnect()
{
    connect(this, &CAbstractEmojiTable::cellClicked, this, &CAbstractEmojiTable::oncellClicked);
}

void CAbstractEmojiTable::oncellClicked(int row,int column)
{
    if (row<0 || column<0 || row>rowCount() || column>columnCount()){
        return;
    }
    QLabel * label = dynamic_cast<QLabel *>(cellWidget(row, column));
    if (label != Q_NULLPTR){
        label->setStyleSheet("background-color:white");
        hide();
        emit insertEmoji(label->toolTip());
    }
}
