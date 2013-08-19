#ifndef TST_PLUGIN4_H
#define TST_PLUGIN4_H

#include <QObject>
#include <plugin/iplugin.h>
#include "tst_plugin4global.h"
#include "itst_plugin4.h"

class  TST_PLUGIN4LIB tst_plugin4:
        public QObject,
        public IPlugin,
        public Itst_plugin4
{
    Q_OBJECT
    Q_INTERFACES(IPlugin Itst_plugin4)
    
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.tst_plugin4" FILE "tst_plugin4.json")
#endif
    
public:
    
    //! Конструктор плагина
    explicit tst_plugin4(QObject *parent = 0);
    
    // IPlugin
    
    //! Получение экземпляра
    QObject *instance() { return this; }
    
    //! Получение имени плагина
    QString name() {return tr("tst_plugin4");};
    
    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/tst_plugin4");}
    
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

