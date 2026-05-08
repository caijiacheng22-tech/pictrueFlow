#ifndef PICTRUEITEM_H
#define PICTRUEITEM_H
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QPixmap>
#include "graphicspictrue_global.h"
class   PictrueItem : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit PictrueItem(QGraphicsItem *parent = Q_NULLPTR);
    explicit PictrueItem(const QPixmap &pixmap, QGraphicsItem *parent = Q_NULLPTR);
    virtual ~PictrueItem();
    void setPixmap(const QPixmap &pixmap);
    QPixmap pixmap() const;
    virtual QRectF boundingRect() const;
    void setTransformationMode(Qt::TransformationMode mode);
    QPointF offset() const;
    void setOffset(const QPointF &offset);
    virtual int type()const;
    void setType(int type);
    int itemId()const;
    void setItemId(int id);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

Q_SIGNALS:

    void clicked();
    void clickedId(int);

private:

    QPointF pressedScenePoint;
    QPointF m_offset;
    QPointF m_pos;
    Qt::TransformationMode mode;
    QPixmap m_pixmap;
    bool isPressed;
    int m_type;
    int m_id;
    qreal m_pointPercent;
};

#endif // PICTRUEITEM_H
