#include "filtertreewidget.h"
#include <QDropEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>

namespace RTPTechGroup {
namespace Widgets {

FilterTreeWidget::FilterTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
     setAcceptDrops(true);



     //setDragEnabled(true);
     //setDragDropMode(QAbstractItemView::DragDrop);
}

bool FilterTreeWidget::checkFormat(QDropEvent *ev)
{
    /*QStringList supportedFormats = GetFormat();
    for (const QString format : qAsConst(supportedFormats))
    {
	if (ev->mimeData()->hasFormat(format))
	{ */
	   ev->acceptProposedAction();
	   return true;
	/*}
    }
    ev->ignore();
    return false; */
}

void  FilterTreeWidget::mousePressEvent(QMouseEvent *pe){
	if (pe->button() == Qt::LeftButton){
		m_ptDragPos=pe->pos();
	}
	QTreeWidget::mousePressEvent(pe);

}

void  FilterTreeWidget::mouseMoveEvent(QMouseEvent *pe){

	if (pe->buttons() & Qt::LeftButton){
		qint32 distance = (pe->pos()-m_ptDragPos).manhattanLength();
		if (distance > QApplication::startDragDistance())
			startDrag();
	}
	QTreeWidget::mouseMoveEvent(pe);
}

void  FilterTreeWidget::startDrag(){
    if (this->currentItem() == nullptr)
	return;
    QMimeData *mimeData = new QMimeData();

    mimeData->setImageData(this->currentItem()->icon(0));
    mimeData->setText(this->objectName());

    QDrag *drag = new QDrag(this);
    drag->setPixmap(this->currentItem()->icon(0).pixmap(32,32,QIcon::Normal));
    drag->setMimeData(mimeData);
    drag->exec(Qt::MoveAction);
}

bool FilterTreeWidget::searchShowItem(QString text,QTreeWidgetItem *item){
    bool flag = false;
    for (qint32 i = item->childCount() - 1; i >= 0; --i)
        if (searchShowItem(text,item->child(i)) && !flag)
            flag = true;

    if (!flag) {
        for (qint32 i = item->columnCount() - 1; i >= 0; --i)
            if (item->text(i).contains(text, Qt::CaseInsensitive)) {
                flag=true;
                break;
            }
    }
    item->setHidden(!flag);

    return flag;
}

void FilterTreeWidget::searchShowAllItem(QString text){
    for (qint32 i = this->topLevelItemCount() - 1; i >= 0; --i)
        searchShowItem(text, this->topLevelItem(i));
}

void FilterTreeWidget::showAllItem(){
    for (qint32 i = this->topLevelItemCount() - 1; i >= 0; --i)
        this->topLevelItem(i)->setHidden(true);
}

void FilterTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
	checkFormat(event);
}

void FilterTreeWidget::dragMoveEvent(QDragMoveEvent *ev)
{

    checkFormat(ev);
}

void FilterTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
    // Вызов меню
   //  menu.exec(event->globalPos());
    emit contextMenuSignal(event);
}

}}
