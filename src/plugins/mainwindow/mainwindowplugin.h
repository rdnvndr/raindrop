#ifndef MAINWINDOWSPLUGIN_H
#define MAINWINDOWSPLUGIN_H

#include "mainwindow_global.h"
#include "iplugin.h"
#include "imainwindow.h"
#include "mainwindow.h"

#include <QPointer>

class MAINWINDOWSSHARED_EXPORT MainWindowPlugin: public IPlugin,
                                                                              public IMainWindow
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
    Q_INTERFACES(IMainWindow)

public:
    MainWindowPlugin(QObject *parent = 0);

    ~MainWindowPlugin();

    //! Инициализация плагина главного окна
    bool initialize();

    bool release();

    //! Чтение и применение настроек плагина главного окна
    void readSettings();

    //! Запись настроек плагина главного окна
    void writeSettings();

    QMainWindow *mainWindow();
private:

     //! Главное окно
    QPointer<MainWindow> m_mainWindows;
};

#endif // MAINWINDOWSPLUGIN_H
