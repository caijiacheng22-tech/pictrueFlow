#pragma once

#ifndef PICTRUERADIOBUTTON_H
#define PICTRUERADIOBUTTON_H
#include <QAbstractButton>
#include "graphicspictrue_global.h"

class   PictrueButton : public QAbstractButton
{
    Q_OBJECT

public:
     explicit PictrueButton(QWidget *parent = Q_NULLPTR);
    ~PictrueButton();
    int id()const;
    void setId(int id);
Q_SIGNALS:
    void entered();
    void entered(int);
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
private:
    bool m_isSelected;
    int m_id;
};

#endif // PICTRUERADIOBUTTON_H
