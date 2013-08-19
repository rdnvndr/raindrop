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

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.tst_plugin2" FILE "tst_plugin2.json")
#endif
    
public:
    
    //! Конструктор плагина
    explicit tst_plugin2(QObject *parent = 0);
    
    // IPlugin
    
    //! Получение экземпляра
    QObject *instance() { return this; }
    
    //! Получение имени плагина
    QString name() {return tr("tst_plugin2");};
    
    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/tst_plugin2");}
    
    //! Описание плагина
    QString descript() {return tr(" ");};
    
     //! Категория в которой состоит плагин
    QString category() {return tr(" ");};
    
    //! Версия плагина
    QString version() {return tr("1.0");};
    
    //! Производитель плагина
    QString vendor() {return tr("RTPTechGroup");};
};

#endif

