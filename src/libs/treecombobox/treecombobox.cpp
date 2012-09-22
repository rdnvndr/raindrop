#include "treecombobox.h"

TreeComboBox::TreeComboBox(QWidget *parent):QComboBox(parent),skipNextHide(false)
{

    QComboBox::resize(250,30);
    treeView = new QTreeView();
    setView(treeView);
    view()->viewport()->installEventFilter(this);
    view()->setProperty("headerHidden",true);
    view()->setMinimumSize(0,150);
    view()->setAlternatingRowColors(true);

}

TreeComboBox::~TreeComboBox()
{
    delete treeView;
}

bool TreeComboBox::eventFilter(QObject*object,QEvent*event)
{
    if(event->type()==QEvent::MouseButtonPress&&object==view()->viewport())
    {
        QMouseEvent*mouseEvent=static_cast<QMouseEvent*>(event);
        QModelIndex index=view()->indexAt(mouseEvent->pos());
        if(!view()->visualRect(index).contains(mouseEvent->pos()))
            skipNextHide=true;
    }
    return false;
}

void TreeComboBox::showPopup()
{
    QComboBox::showPopup();
}

void TreeComboBox::hidePopup()
{
    setRootModelIndex(view()->currentIndex().parent());
    setCurrentIndex(view()->currentIndex().row());

    if(skipNextHide)skipNextHide=false;
    else QComboBox::hidePopup();

    view()->setRootIndex(QModelIndex());
}

void TreeComboBox::setModel(QAbstractItemModel *model)
{
    QComboBox::setModel(model);
    for (int i=1;i<treeView->header()->count();i++)
        treeView->hideColumn(i);
}

void TreeComboBox::setCurrentModelIndex(const QModelIndex &index)
{
    treeView->setCurrentIndex(index);
}
