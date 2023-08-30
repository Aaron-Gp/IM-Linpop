#include "emojitab.h"
#include <QTableWidget>
#include <QTabWidget>

EmojiTab::EmojiTab(QWidget *parent) : QWidget(parent)
{
    m_tab = new QTabWidget;
    m_tab->setMovable(false);
    m_tab->setTabsClosable(false);
    QVector<char32_t> emoji;
    for(char32_t i = 0x1F601 ; i <=  0x1F64F ; ++i)
    {
        emoji.append(i);
    }

    QTableWidget *table = new QTableWidget;

}
