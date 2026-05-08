#include "pictruebutton.h"
#include <QPen>
#include <QPainter>
#include <QDebug>
#include <QPainterPath>
PictrueButton::PictrueButton(QWidget *parent)
    :QAbstractButton(parent),
     m_isSelected(false),
     m_id(0)
{
    setCheckable(true);
    setFixedSize(50,10);
}

PictrueButton::~PictrueButton()
{

}

int PictrueButton::id() const
{
    return m_id;
}

void PictrueButton::setId(int id)
{
    m_id = id;
}

void PictrueButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRectF drawRect = this->rect();
    QPainterPath drawPath;
    //qDebug()<<drawRect;
    QPen drawPen;
    drawPen.setWidth(3);
    //选中为蓝,未选中为灰
    drawPen.setColor(Qt::gray);
    painter.setPen(drawPen);
    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    drawPath.addRoundedRect(drawRect,10,10);
    painter.setClipPath(drawPath);
    if(isChecked())
        painter.fillRect(drawRect,QColor(0,0,255,128));
    else
        painter.fillRect(drawRect,QColor(128,128,128,128));
}

void PictrueButton::enterEvent(QEvent *event)
{
    qDebug()<<"enter";
    if(!isChecked())
        setChecked(true);
    emit entered(m_id);
    return QAbstractButton::enterEvent(event);
}

void PictrueButton::leaveEvent(QEvent *event)
{
    return QAbstractButton::leaveEvent(event);
}
