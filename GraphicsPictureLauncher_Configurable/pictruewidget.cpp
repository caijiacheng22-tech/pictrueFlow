#include "pictruewidget.h"
#include "pictrueitem.h"
#include <QGraphicsScene>
PictrueWidget::PictrueWidget()
{

}

void PictrueWidget::addPicture(QPixmap &pixmap)
{
    PictrueItem *item = new PictrueItem(pixmap);
    m_scene->addItem(item);

}

void PictrueWidget::addPicture(QPixmap &pixmap, int position)
{

}

void PictrueWidget::removePicture(int position)
{

}
