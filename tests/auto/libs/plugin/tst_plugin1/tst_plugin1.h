#ifndef TST_PLUGIN1_H
#define TST_PLUGIN1_H

#include <QObject>
#include <plugin/iplugin.h>
#include "tst_plugin1global.h"
#include "itst_plugin1.h"

class  TST_PLUGIN1LIB tst_plugin1:
        public QObject,
        public IPlugin,
        public Itst_plugin1
{
    Q_OBJECT
    Q_INTERFACES(IPlugin Itst_plugin1)
    
    Q_PLUGIN_METADATA(IID Itst_plugin1_iid FILE "tst_plugin1.json")
    
public:
    
    //! Конструктор плагина
    explicit tst_plugin1(QObject *parent = 0);
    
    // IPlugin
    
    //! Получение экземпляра
    QObject *instance() { return this; }
    
    //! Получение имени плагина
    QString name() {return APP_NAME;};

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/tst_plugin1");};

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

