#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

#define RAW_VIEW_SIZE QSize(476,414)
#define SCALE_VIEW_PIXMAP (qreal)2/1 //View与图片比例
#define SCALE_BIG_SMALL (qreal)2/1 //图片大小比例
//P1-P8，8个位置，根据需要改动
#define P1 (qreal)0.00
#define P2 (qreal)0.25
#define P3 (qreal)0.50
#define P4 (qreal)0.75
#define P5 (qreal)1.00
#define P6 P4
#define P7 P3
#define P8 P2
#define MID_TYPE 2
#define FPS 60//帧数，每秒
#define DURATION_MS 500//移动一次图元经过时间,毫秒，不得低于帧数

class QGraphicsScene;
class QButtonGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum Rules :int {
        RuleA = 1,
        RuleB = -1,
        RuleC = 2,
        RuleD = -2
    };

    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    int getIndexByRules(int oldIndex, int rule);
    template<typename T>
    void rollList(QList<T>& oldList, int dir, int count);
    void rollItem(int rollDir, unsigned rollCount);
public Q_SLOTS:
    void timerOutFunc();
    void nextItem();
    void previousItem();
    void clickedItemRoll(int type);
protected:

private:
    Ui::MainWindowClass ui;
    QTimer* m_timer;
    QGraphicsScene* m_scene;
    QLineF midLine;
    QList<qreal> pointList;
    QList<QPixmap> pixmapList;
    QList<qreal> zValueList;
    QList<qreal> pixmapScaleList;
    int m_index;
    Rules currentRule;
    unsigned m_rollCount;
    QButtonGroup* btnGroup;
    bool btnMoveEnable;
};

#endif // MAINWINDOW_H

