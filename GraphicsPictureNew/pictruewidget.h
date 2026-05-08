#ifndef PICTRUEWIDGET_H
#define PICTRUEWIDGET_H

#include <QLineF>
#include <QList>
#include "graphicspictrue_global.h"

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

class PictrueView;
class QPixmap;
class QButtonGroup;
class QTimer;
class QGraphicsScene;

class   PictrueWidget
{
public:
    enum Rules:int{
        RuleA = 1,
        RuleB = -1,
        RuleC = 2,
        RuleD = -2
    };
public:
    PictrueWidget();
    void addPicture(QPixmap &pixmap);
    void addPicture(QPixmap &pixmap,int position);
    void removePicture(int position);
private:

    QTimer *m_timer; //定时器
    QGraphicsScene *m_scene; //场景
    PictrueView *m_view; //视图
    QLineF m_MidLine; //中等线，确定图片位置
    QList<qreal> m_PointList; //各个图片位置信息
    QList<QPixmap> m_PixmapList; //各个图片列表
    QList<qreal> m_ZValueList; //各个显示优先级列表
    QList<qreal> m_PixmapScaleList; //各个图片位置信息伸缩因子列表
    //int m_index;
    Rules m_currentRule; //当前执行的类型操作
    unsigned m_rollCount; //滚动次数
    QButtonGroup *m_BtnGroup; //按钮盒子
    //bool btnMoveEnable;
};

#endif // PICTRUEWIDGET_H
