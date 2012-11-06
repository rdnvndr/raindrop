#ifndef MAINWINDOWPLUG_H
#define MAINWINDOWPLUG_H

#include <plugin/iplugin.h>
#include "mainwindow.h"


//! Плагин главного окна
/*! Плагин предназначен для создания главного окна
    Весь графический интерфейс приложения должен строится
    на базе данного окна. Поддерживает закладочный и оконный
    интерфейс.\n
    Пример:
    \code
        PluginManager* pluginManager = PluginManager::instance();
        MainWindow* mainwindow = qobject_cast<MainWindow*>(pluginManager->getObjectByName(
                                               "MainWindowPlug::MainWindow"));
        mainwindow->addSubWindow(new QTextEdit(NULL));
    \endcode
*/
class MainWindowPlug: public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
public:
    //! Конструктор плагина главного окна
    MainWindowPlug(QObject *parent = 0);
    virtual ~MainWindowPlug();


    //! Главное окно
    MainWindow *window;

    //! Инициализация плагина главного окна
    bool initialize();

    //! Освобождение плагина главного окна
    bool release();

    //! Чтение и применение настроек плагина главного окна
    void readSettings();

    //! Запись настроек плагина главного окна
    void writeSettings();

};

#endif
