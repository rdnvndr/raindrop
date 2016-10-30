#ifndef PLUG_EXAMPLE_H
#define PLUG_EXAMPLE_H

#include <plugin/iplugin.h>
#include <imainwindow.h>
#include <itreedockwidget.h>
#include "exampleglobal.h"

namespace RTPTechGroup {
namespace Example {

class EXAMPLELIB ExamplePlug:
        public QObject,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IPlugin_iid FILE "example.json")
#endif

public:

    explicit ExamplePlug(QObject *parent = 0);

// IPlugin

    //! Получение имени плагина
    QString name() {return APP_NAME;}

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/example");}

    //! Описание продукта
    QString product() {return tr(APP_PRODUCT);}

    //! Авторские права
    QString copyright() {return tr(APP_COPYRIGHT);}

    //! Описание плагина
    QString descript() {return tr(APP_DESCRIPTION);}

    //! Категория в которой состоит плагин
    QString category() {return tr("");}

    //! Версия плагина
    QString version() {return APP_VERSION;}

    //! Производитель плагина
    QString vendor() {return tr(APP_COMPANY);}

public slots:

    //! Создание закладки
    void createTab();

    //! Создание диалогового окна
    void createDlgWindow();

};

}}

#endif
