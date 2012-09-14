#ifndef IMAINWINDOW_H
#define IMAINWINDOW_H

#include <QMainWindow>

//! Интерфейс  главного окна
class IMainWindow
{
public:
    virtual QMainWindow* mainWindow() = 0;
};


Q_DECLARE_INTERFACE(IMainWindow,"com.RTPTechGroup.UniversalPlatform.IMainWindow/1.0")
#endif // IMAINWINDOW_H
