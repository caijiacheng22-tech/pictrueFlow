#include "pictrueitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>
PictrueItem::PictrueItem(QGraphicsItem *parent)
    :QGraphicsObject(parent),
     isPressed(false),
     mode(Qt::SmoothTransformation),
     m_type(0),
     m_id(0),
     m_pointPercent((qreal)0.0)
{

}

PictrueItem::PictrueItem(const QPixmap &pixmap, QGraphicsItem *parent)
    :QGraphicsObject(parent),
     isPressed(false),
     mode(Qt::SmoothTransformation),
     m_type(0)
{
    m_pixmap = pixmap;
}

PictrueItem::~PictrueItem()
{

}

void PictrueItem::setPixmap(const QPixmap &pixmap)
{
    prepareGeometryChange();
    m_pixmap = pixmap;
    update();
}

QPixmap PictrueItem::pixmap() const
{
    return m_pixmap;
}

QRectF PictrueItem::boundingRect() const
{
    if(m_pixmap.isNull())
        return QRectF();
    return QRectF(m_offset, m_pixmap.size() / m_pixmap.devicePixelRatio());
}

void PictrueItem::setTransformationMode(Qt::TransformationMode mode)
{
    if (mode != this->mode)
    {
        this->mode = mode;
        update();
    }
}

QPointF PictrueItem::offset() const
{
    return m_offset;
}

void PictrueItem::setOffset(const QPointF &offset)
{
    m_offset = offset;this->pos();
    if (m_offset == offset)
        return;
    prepareGeometryChange();
    m_offset = offset;
    update();
}

int PictrueItem::type() const
{
    return m_type;
}

void PictrueItem::setType(int type)
{
    m_type = type;
}

int PictrueItem::itemId() const
{
    return m_id;
}

void PictrueItem::setItemId(int id)
{
    m_id = id;
}



void PictrueItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //只响应鼠标左键
    if(event->button() == Qt::LeftButton)
    {
        pressedScenePoint = event->pos();
        isPressed = true;

    }
}

void PictrueItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(isPressed &&
                boundingRect().contains(event->pos()) &&
                boundingRect().contains(pressedScenePoint))
        {
            isPressed = false;
            emit clicked();
            emit clickedId(type());
        }
    }
}

void PictrueItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    painter->setRenderHint(QPainter::SmoothPixmapTransform,
                           (this->mode == Qt::SmoothTransformation));

    painter->drawPixmap(m_offset, m_pixmap);

}
