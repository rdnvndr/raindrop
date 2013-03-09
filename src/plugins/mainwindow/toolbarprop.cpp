#include "toolbarprop.h"

ToolBarProp::ToolBarProp(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

QString ToolBarProp::text()
{
    return lineEditName->text();
}


void ToolBarProp::setText(QString text)
{
    lineEditName->setText(text);
}
