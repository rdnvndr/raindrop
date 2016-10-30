#ifndef PGDATABASEMODEL_H
#define PGDATABASEMODEL_H

#include <QObject>
#include <plugin/iplugin.h>
#include "pgdatabasemodelglobal.h"

#include <idatabasemodelbuilder.h>
#include "pgdatabaseattr.h"


//! Реализация модели базы данных PostgreSql
//!
class  PGDATABASEMODELLIB PgDatabaseModel:
        public QObject,
        public IDatabaseModelBuilder,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin IDatabaseModelBuilder)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.PgDatabaseModel" FILE "pgdatabasemodel.json")
#endif

public:

    //! Конструктор плагина
    explicit PgDatabaseModel(QObject *parent = 0);

    //! Деструктор плагина
    virtual ~PgDatabaseModel();

// IPlugin
    //! Получение имени плагина
    QString name() {return APP_NAME;}

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/pgdatabasemodel");}

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

// IDatabaseModelBuilder
    //! Создание экземпляра класса
    IDatabaseClass *createClass();

    //! Создание экземпляра атрибута
    IDatabaseAttribute *createAttr();

    //! Создание экземпляра фильтра
    IDatabaseFilter *createFilter();

    //! Создание экземпляра состава
    IDatabaseComposition *createComposition();

    //! Имя строителя модели базы данных
    QString modelBuilderName();

};

#endif
