#include "pictruecntroller.h"

PictrueCntroller::PictrueCntroller(QObject *parent)
	: QObject(parent)
{}

PictrueCntroller::~PictrueCntroller()
{}
void PictrueCntroller::setPictureItems(const QList<PictrueItem*> itemList)
{
	m_item_list = itemList;
}

int PictrueCntroller::getCurrentPictureID()const
{
	return m_current_picture_id;
}
void PictrueCntroller::nextItem()
{
	rollItem(RollDir::Positive);
}
void PictrueCntroller::previousItem()
{
	rollItem(RollDir::Negative);
}
void PictrueCntroller::setCurrentPictureID(int id)
{
	if (m_item_list.count() < id)
		return;
	m_current_picture_id = id;
}
void PictrueCntroller::rollItem(RollDir dir)
{

}