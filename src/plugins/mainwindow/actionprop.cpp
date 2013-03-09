#include "actionprop.h"
#include <QFileDialog>

ActionProp::ActionProp(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    connect(pushButtonOpen,SIGNAL(clicked()),pushButtonIcon,SLOT(openImage()));
    connect(pushButtonDelete,SIGNAL(clicked()),pushButtonIcon,SLOT(cleanImage()));
}
