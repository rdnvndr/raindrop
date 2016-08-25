#ifndef PGDATABASEMODEL_H
#define PGDATABASEMODEL_H

#include <QObject>
#include <plugin/iplugin.h>
#include <idatabasemodel.h>
#include "pgdatabasemodelglobal.h"

class  PGDATABASEMODELLIB PgDatabaseModel:
        public QObject,
        public IPlugin,
        public IDatabaseModel
{
    Q_OBJECT
    Q_INTERFACES(IPlugin IDatabaseModel)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.PgDatabaseModel" FILE "pgdatabasemodel.json")
#endif

public:

    //! Конструктор плагина
    explicit PgDatabaseModel(QObject *parent = 0);

    // IPlugin

    //! Получение экземпляра
    QObject *instance() { return this; }

    //! Получение имени плагина
    QString name() {return tr("pgdatabasemodel");};

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/pgdatabasemodel");}

    //! Описание продукта
    QString product() {return APP_PRODUCT;};

    //! Авторские права
    QString copyright() {return APP_COPYRIGHT;};

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
