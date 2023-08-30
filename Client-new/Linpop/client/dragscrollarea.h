#ifndef _DRAG_SCROLL_AREA_H__
#define _DRAG_SCROLL_AREA_H__
#include <QScrollArea>

class DragScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    DragScrollArea(QWidget *parent = Q_NULLPTR);
    ~DragScrollArea();

protected:
    virtual bool event(QEvent* event);
};
#endif //_DRAG_SCROLL_AREA_H__
