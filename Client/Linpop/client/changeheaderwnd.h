#ifndef _CHANGE_HEADER_WND_H__
#define _CHANGE_HEADER_WND_H__

#include <QWidget>
#include "ui_ChangeHeaderWnd.h"

class ChangeHeaderWnd : public QWidget
{
    Q_OBJECT

public:
    ChangeHeaderWnd(QWidget *parent = Q_NULLPTR);
    ~ChangeHeaderWnd();

    void LoadHeader(const QPixmap& pixmap);
    void RotateHeader(bool bClock);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event);

signals:
    void updateHeader(const QPixmap& header);

protected slots:
    void onUpload();
    void onScaledChanged(int nValue);
    void onOk();
    void onCancel();

private:
    Ui::ChangeHeaderWnd ui;
    QWidget*			m_pDivWidget;			//选择头像区域的遮罩层
    QPixmap				m_sourceHeader;			//上传的原始头像文件
};
#endif //_CHANGE_HEADER_WND_H__
