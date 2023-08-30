#ifndef EMOJITAB_H
#define EMOJITAB_H

#include <QMainWindow>
#include <QWidget>

class QTabWidget;

class EmojiTab : public QWidget
{
    Q_OBJECT
public:
    explicit EmojiTab(QWidget *parent = nullptr);
    QTabWidget *m_tab;


signals:

};

#endif // EMOJITAB_H
