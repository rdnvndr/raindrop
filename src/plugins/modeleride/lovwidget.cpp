#include "lovwidget.h"

LovWidget::LovWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

bool LovWidget::isRemove(const QModelIndex &srcIndex)
{
    return false;
}

void LovWidget::setModel(TreeXmlHashModel *model)
{

}

void LovWidget::setCurrentLov(const QModelIndex &index)
{

}
