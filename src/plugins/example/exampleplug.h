#ifndef PLUG_EXAMPLE_H
#define PLUG_EXAMPLE_H

#include <plugin/iplugin.h>
#include <imainwindow.h>
#include <itreedockwidget.h>
#include "exampleglobal.h"

class EXAMPLELIB ExamplePlug:
        public QObject,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)


#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.Example" FILE "example.json")
#endif

public:

    ExamplePlug(QObject *parent = 0);

// IPlugin
    //! Инициализация плагина
    bool initialize();

    //! Освобожение плагина
    bool release();

    //! Получение экземпляра
    QObject *instance() { return this; }

public slots:

    //! Создание закладки
    void createTab();

    //! Создание диалогового окна
    void createDlgWindow();

};

#endif
