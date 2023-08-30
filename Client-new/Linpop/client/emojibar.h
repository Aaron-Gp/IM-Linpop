#ifndef EMOJIBAR_H
#define EMOJIBAR_H

#include <QWidget>
#include "global.h"

#include <QTableWidget>;

class EmojiBar : public QWidget
{
    Q_OBJECT
public:
    explicit EmojiBar(QWidget *parent = nullptr);

    void showEmoji(QRect geo);
    QTableWidget *tableWidget;


signals:

};

#endif // EMOJIBAR_H
