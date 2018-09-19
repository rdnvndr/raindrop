#ifndef PGDATABASEMODELBUILDER_H
#define PGDATABASEMODELBUILDER_H

#include <QObject>
#include <plugin/iplugin.h>
#include "pgdatabasemodelglobal.h"

#include <idatabasemodelbuilder.h>
#include <idatabasemodel.h>

#include "pgdatabaseattribute.h"

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация модели базы данных PostgreSql
class  PGDATABASEMODELLIB PgDatabaseModelBuilder:
        public QObject,
        public IDatabaseModelBuilder,
        public IPlugin
{
    Q_OBJECT

    Q_INTERFACES(IPlugin IDatabaseModelBuilder)
    Q_PLUGIN_METADATA(IID IDatabaseModelBuilder_iid FILE "pgdatabasemodel.json")

public:

    //! Конструктор плагина
    explicit PgDatabaseModelBuilder(QObject *parent = nullptr);

    //! Деструктор плагина
    virtual ~PgDatabaseModelBuilder();

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
    //! Создание экземпляра модели базы данных
    IDatabaseModel *createDatabaseModel(IDatabasePool *pool);

    //! Модель базы данных
    QString implDriverName() const;

};

}}

#endif
