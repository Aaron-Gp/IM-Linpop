#include "emojibar.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QHeaderView>

EmojiBar::EmojiBar(QWidget *parent) : QWidget(parent)
{

}

void EmojiBar::showEmoji(QRect geo)
{
    QString emojis[]={"😄","😆","😊","😃","😏","😍","😘","😚","😳","😌","😆","😁","😉","😜","😝","😀","😗","😙","😛","😴","😟","😦","😧","😮","😬","😕","😯","😑","😒","😅","😓","😥","😩","😔","😞","😖","😨","😰","😣","😢","😭","😂","😲","😱","😫","😠","😡","😤","😪","😋","😷","😎","😵","👿","😈","😐","😶","😇"};
    tableWidget = new QTableWidget;
    tableWidget->setFixedSize(300,150);//设置大小
//    QRect rectbtn = pushButton->geometry();
    QRect rectTable = geo;
    rectTable.setX(geo.x()-100);
    rectTable.setY(geo.y()-150);
    tableWidget->setGeometry(rectTable);//设置位置在按钮上面
    tableWidget->setRowCount(5); // 设置行数
    tableWidget->setColumnCount(5); // 设置列数
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);//固定行高
    tableWidget->verticalHeader()->setDefaultSectionSize(50);//设置行高50
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);//固定列宽
    tableWidget->horizontalHeader()->setDefaultSectionSize(50);//设置列宽50
    tableWidget->horizontalHeader()->setVisible(false); //隐藏水平表头
    tableWidget->verticalHeader()->setVisible(false); //隐藏垂直表头
    //tableWidget->horizontalScrollBar()->setDisabled(true); // 禁用滚动
    tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏水平滚动条
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // 设置多选模式
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // 设置禁止编辑

    QFont font;
    font.setPointSize(35);
    for(int i=0;i<15;i++){
        QTableWidgetItem *item=new QTableWidgetItem();
        item->setFont(font);
        item->setText(emojis[i]);
        item->setSizeHint(QSize(50,50));
        item->setTextAlignment(Qt::AlignHCenter);
        tableWidget->setItem(i/5,i%5,item);
    }
    /*
        QString address=":/emojis/"+QString::number(i+1)+".jpg";//图片地址
        QPixmap originalPixmap(address);
        QPixmap scaledPixmap = originalPixmap.scaled(QSize(50, 50)); // 设置尺寸为 50x50 像素
        QLabel *lable = new QLabel();     					//创建lable
        lable->setPixmap(scaledPixmap);    //加载图片到QLable
        lable->setAlignment(Qt::AlignHCenter);      		//设置QLable控件居中显示
        tableWidget->setCellWidget(i/5,i%5,lable);     		//显示添加QLable到控件*/
    tableWidget->setVisible(true);
//    connect(ui->pushButton,&QPushButton::clicked,[=]{//按一下显示，再按一下关闭
//        if(!tableWidget->isVisible())
//            tableWidget->setVisible(true);
//        else{
//            tableWidget->setVisible(false);
//        }
//    });
//    connect(tableWidget, &QTableWidget::itemSelectionChanged, this, [=](){
//        // 获取当前选中的行和列
//        int row = tableWidget->currentRow();
//        int column = tableWidget->currentColumn();
//        qDebug() << row;
//        qDebug() << column;
//        // 获取选中项的数据
//        QTableWidgetItem *item = tableWidget->item(row,column);
//        qDebug() << item;
//        if (item) {
//            // 使用QTableWidgetItem的方法获取数据
//            QString data = item->text();
//            // 在这里处理数据...
//            qDebug() << data;
//            qDebug() << "yes";
//        }
//        qDebug() << "no";
//    });
}
