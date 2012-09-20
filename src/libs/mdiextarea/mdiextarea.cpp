#include "mdiextarea.h"

MdiExtArea::MdiExtArea(QWidget *parent) :
    QMdiArea(parent)
{
    setViewMode(QMdiArea::TabbedView);
}

QMdiSubWindow *MdiExtArea::addSubWindow(QWidget *widget, Qt::WindowFlags flags){
    QMdiSubWindow *subwindow = QMdiArea::addSubWindow(widget,flags);

    if (viewMode() == QMdiArea::TabbedView){
        QTabBar *tabBar = findChild <QTabBar*>();
        if (!tabBar->tabsClosable())
            tabBar->setTabsClosable(true);
    }
    subwindow->setAttribute(Qt::WA_DeleteOnClose);
    subwindow->showMaximized();

    return subwindow;
}

void MdiExtArea::setViewMode(ViewMode mode){
    if (mode == QMdiArea::TabbedView)
        for (int i=0;i<this->subWindowList().count();i++)
            this->subWindowList().at(i)->showMaximized();

    QMdiArea::setViewMode(mode);
    if (mode == QMdiArea::TabbedView){
        QTabBar *tabBar = findChild <QTabBar*>();
        if (!tabBar->tabsClosable()){
            tabBar->setTabsClosable(true);
            tabBar->parentWidget()->adjustSize();
        }
        if (activeSubWindow()!=NULL)
            activeSubWindow()->showMaximized();
    }
    this->setOption(QMdiArea::DontMaximizeSubWindowOnActivation,true);
}

QMdiSubWindow* MdiExtArea::subWindow(QString widgetName) {
    foreach (QMdiSubWindow * subwindow, this->subWindowList()){
        if (subwindow->widget()->objectName()==widgetName){
            return subwindow;
        }
    }
    return NULL;
}

QMdiSubWindow* MdiExtArea::setActiveSubWindow(QString widgetName)
{
    QMdiSubWindow* window  = this->subWindow(widgetName);
    if (window){
        this->setActiveSubWindow(window);
        return window;
    }
    return NULL;
}

void MdiExtArea::setActiveSubWindow(QMdiSubWindow *window)
{
    QMdiArea::setActiveSubWindow(window);
}

void MdiExtArea::closeActiveSubWindow(){
    QMdiSubWindow* subWindow = activeSubWindow();
    activateNextSubWindow();
    removeSubWindow(subWindow);
}

void MdiExtArea::closeAllSubWindows(){
    QMdiSubWindow* subWindow = activeSubWindow();
    while (subWindow){
        activateNextSubWindow();
        removeSubWindow(subWindow);
        subWindow = activeSubWindow();
    }
}

void MdiExtArea::cascadeSubWindows(){
     if (viewMode() == QMdiArea::TabbedView)
         return;
     else
         QMdiArea::cascadeSubWindows();

}

void MdiExtArea::tileSubWindows(){
    if (viewMode() == QMdiArea::TabbedView)
        return;
    else
        QMdiArea::tileSubWindows();
}
