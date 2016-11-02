#ifndef DBMODELEXAMPLE_H
#define DBMODELEXAMPLE_H

#include <QObject>
#include <plugin/iplugin.h>
#include "dbmodelexampleglobal.h"

class  DBMODELEXAMPLELIB DbModelExample:
        public QObject,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.DbModelExample" FILE "dbmodelexample.json")
#endif

public:

    //! Конструктор плагина
    explicit DbModelExample(QObject *parent = 0);

    // IPlugin

    //! Получение имени плагина
    QString name() {return APP_NAME;}

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/dbmodelexample");}

    //! Описание продукта
    QString product() {return APP_PRODUCT;}

    //! Авторские права
    QString copyright() {return APP_COPYRIGHT;}

    //! Описание плагина
    QString descript() {return tr(APP_DESCRIPTION);}

    //! Категория в которой состоит плагин
    QString category() {return tr("DatabaseModel");}

    //! Версия плагина
    QString version() {return APP_VERSION;}

    //! Производитель плагина
    QString vendor() {return tr(APP_COMPANY);}
};

#endif