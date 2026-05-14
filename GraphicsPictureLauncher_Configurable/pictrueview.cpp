#include "pictrueview.h"
#include <QResizeEvent>
PictrueView::PictrueView(QWidget *parent)
    :QGraphicsView(parent)
{

}

PictrueView::~PictrueView()
{
    //none
}

void PictrueView::resizeEvent(QResizeEvent *event)
{
    emit sizeChanged(event->size());
    return QGraphicsView::resizeEvent(event);
}
