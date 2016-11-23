#ifndef TST_PLUGIN2_H
#define TST_PLUGIN2_H

#include <QObject>
#include <plugin/iplugin.h>
#include "tst_plugin2global.h"
#include "itst_plugin2.h"

class  TST_PLUGIN2LIB tst_plugin2:
        public QObject,
        public IPlugin,
        public Itst_plugin2
{
    Q_OBJECT
    Q_INTERFACES(IPlugin Itst_plugin2)

    Q_PLUGIN_METADATA(IID Itst_plugin2_iid FILE "tst_plugin2.json")
    
public:
    
    //! Конструктор плагина
    explicit tst_plugin2(QObject *parent = 0);
    
    // IPlugin
    
    //! Получение экземпляра
    QObject *instance() { return this; }
    
    //! Получение имени плагина
    QString name() {return APP_NAME;};

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/tst_plugin2");};

    //! Описание продукта
    QString product() {return tr(APP_PRODUCT);};

    //! Авторские права
    QString copyright() {return tr(APP_COPYRIGHT);};

    //! Описание плагина
    QString descript() {return tr(APP_DESCRIPTION);};

    //! Категория в которой состоит плагин
    QString category() {return tr("");};

    //! Версия плагина
    QString version() {return APP_VERSION;};

    //! Производитель плагина
    QString vendor() {return tr(APP_COMPANY);};
};

#endif

