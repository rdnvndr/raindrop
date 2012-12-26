#include "filtertreewidget.h"
#include <QDropEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>

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
    foreach (QString format, supportedFormats)
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

void  FilterTreeWidget::mousePressEvent(QMouseEvent* pe){
	if (pe->button() == Qt::LeftButton){
		m_ptDragPos=pe->pos();
	}
	QTreeWidget::mousePressEvent(pe);

}

void  FilterTreeWidget::mouseMoveEvent(QMouseEvent* pe){

	if (pe->buttons() & Qt::LeftButton){
		int distance = (pe->pos()-m_ptDragPos).manhattanLength();
		if (distance > QApplication::startDragDistance())
			startDrag();
	}
	QTreeWidget::mouseMoveEvent(pe);
}

void  FilterTreeWidget::startDrag(){
    if (this->currentItem()==NULL)
	return;
    QMimeData* mimeData = new QMimeData();

    mimeData->setImageData(this->currentItem()->icon(0));
    mimeData->setText(this->objectName());

    QDrag* drag = new QDrag(this);
    drag->setPixmap(this->currentItem()->icon(0).pixmap(32,32,QIcon::Normal));
    drag->setMimeData(mimeData);
    drag->start(Qt::MoveAction);
}

bool FilterTreeWidget::searchShowItem(QString text,QTreeWidgetItem* item){
    bool flag = false;
    for (int i=0;i < item->childCount();i++){
	if (searchShowItem(text,item->child(i))||flag)
	    flag = true;
    }

    for (int i=0;i<item->columnCount();i++)
        if (flag==false&&item->text(0).contains(text, Qt::CaseInsensitive))
            flag=true;

    if (flag==false)
        item->setHidden(true);
    else
        item->setHidden(false);
    return flag;
}

void FilterTreeWidget::searchShowAllItem(QString text){

    for (int i=0;i<this->topLevelItemCount();i++)
	searchShowItem(text, this->topLevelItem(i));
}

void FilterTreeWidget::showAllItem(){
    for (int i=0;i<this->topLevelItemCount();i++)
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

