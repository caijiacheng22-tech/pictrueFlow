#ifndef PICTRUEVIEW_H
#define PICTRUEVIEW_H
#include <QGraphicsView>
#include "graphicspictrue_global.h"
class PictrueView : public QGraphicsView
{
    Q_OBJECT

public:
    PictrueView(QWidget *parent = Q_NULLPTR);
    virtual ~PictrueView();
protected:
    void resizeEvent(QResizeEvent *event);
public:
Q_SIGNALS:
    void sizeChanged(const QSize &);
};

#endif // PICTRUEVIEW_H
