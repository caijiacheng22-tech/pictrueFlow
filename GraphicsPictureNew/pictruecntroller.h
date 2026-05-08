#pragma once

#include <QObject>
#include <QTimer>
#include "pictrueitem.h"

class PictrueCntroller  : public QObject
{
	Q_OBJECT

public:
	
	//滚动方向
	enum RollDir :int {
		Positive,//正方向
		Negative//反方向
	};

	PictrueCntroller(QObject *parent);
	~PictrueCntroller();
	//设置图片容器，容器里要存有图片
	void setPictureItems(const QList<PictrueItem*> item_list);
	//获取当前图片的ID，就是目前是第几张图片在中心
	int getCurrentPictureID()const;
	//滚动下一张图片
	void nextItem();
	//滚动上一张图片
	void previousItem();
	//设置滚动界面显示的图片数量
	void setShowPictureCount(uint show_count);

Q_SIGNALS:
	//【信号】当前当前图片的ID（就是目前是第几张图片在中心）已改变
	void currentPictureIDChanged(int id);

private:
	//设置当前图片的ID，就是目前是第几张图片在中心
	void setCurrentPictureID(int id);
	//滚动图片
	void rollItem(RollDir dir);
	//内部图片容器
	QList<PictrueItem*> m_item_list;
	//标志当前图片的ID，就是目前是第几张图片在中心
	int m_current_picture_id;
	//标志滚动界面显示的图片数量
	uint show_count;
	//标志滚动界面显示的中心图片ID
	//定时器，滚动触发时用于定时移动图片，以达成类似动画效果
	QTimer m_timer;
};
