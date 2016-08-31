#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include <QObject>
#include <plugin/iplugin.h>
#include "databasemodelglobal.h"

#include <idatabasemodel.h>

//! Модель базы данных

class  DATABASEMODELLIB DatabaseModel:
        public QObject,
        public IDatabaseModel,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin IDatabaseModel)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.DatabaseModel" FILE "databasemodel.json")
#endif

public:

    //! Конструктор плагина
    explicit DatabaseModel(QObject *parent = 0);

    //! Деструктор плагина
    virtual ~DatabaseModel();

// IPlugin

    //! Получение имени плагина
    QString name() {return APP_NAME;};

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/databasemodel");}

    //! Описание продукта
    QString product() {return APP_PRODUCT;};

    //! Авторские права
    QString copyright() {return APP_COPYRIGHT;};

    //! Описание плагина
    QString descript() {return tr(APP_DESCRIPTION);};

    //! Категория в которой состоит плагин
    QString category() {return tr("DatabaseModel");};

    //! Версия плагина
    QString version() {return APP_VERSION;};

    //! Производитель плагина
    QString vendor() {return tr(APP_COMPANY);};
};

#endif
