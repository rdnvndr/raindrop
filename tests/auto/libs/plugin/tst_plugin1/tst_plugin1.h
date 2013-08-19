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
    
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.tst_plugin1" FILE "tst_plugin1.json")
#endif
    
public:
    
    //! Конструктор плагина
    explicit tst_plugin1(QObject *parent = 0);
    
    // IPlugin
    
    //! Получение экземпляра
    QObject *instance() { return this; }
    
    //! Получение имени плагина
    QString name() {return tr("tst_plugin1");};
    
    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/tst_plugin1");}
    
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

