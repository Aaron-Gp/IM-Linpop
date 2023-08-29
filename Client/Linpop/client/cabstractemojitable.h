#ifndef CABSTRACTEMOJITABLE_H
#define CABSTRACTEMOJITABLE_H

#include <QObject>
#include <QTableWidget>

class CAbstractEmojiTable : public QTableWidget
{
    Q_OBJECT
public:
    CAbstractEmojiTable(QWidget *parent);
    ~CAbstractEmojiTable();
    int setRowCountAndColumnCount(int totalCount,int columnCount=10);
    QMap<QString, QString> code2url;
signals:
    void insertEmoji(const QString &emojicode);
private slots:
    void oncellClicked(int row,int column);
private:
    void addEmoji();
    void initStyle();
    void initConnect();
};

#endif // CABSTRACTEMOJITABLE_H
