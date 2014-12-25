#include "mdisubwindow.h"
#include <QDebug>

MdiSubWindow::MdiSubWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMdiSubWindow(parent,flags)
{

}

MdiSubWindow::~MdiSubWindow()
{

}


void MdiSubWindow::closeEvent(QCloseEvent *event)
{
    if ( this->widget() && this->testAttribute(Qt::WA_DeleteOnClose))
        delete this->widget();
    QMdiSubWindow::closeEvent(event);
}



