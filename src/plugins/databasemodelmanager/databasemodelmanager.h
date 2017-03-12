#ifndef DATABASEMODELMANAGER_H
#define DATABASEMODELMANAGER_H

#include "databasemodelmanagerglobal.h"

#include <QObject>
#include <QtSql/QSqlDatabase>

#include <idatabasemodelmanager.h>
#include <idatabasemodel.h>
#include <plugin/iplugin.h>

//! Менеджер моделей баз данных
class  DATABASEMODELMANAGERLIB DatabaseModelManager:
        public QObject,
        public IDatabaseModelManager,
        public IPlugin
{
    Q_OBJECT

    Q_INTERFACES(IPlugin IDatabaseModelManager)
    Q_PLUGIN_METADATA(IID IDatabaseModelManager_iid FILE "databasemodelmanager.json")

public:

    //! Конструктор плагина
    explicit DatabaseModelManager(QObject *parent = 0);

    //! Деструктор плагина
    virtual ~DatabaseModelManager();

// IPlugin

    //! Получение имени плагина
    QString name() {return APP_NAME;}

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/databasemodel");}

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

// IDatabaseModel

    //! Получение модели базы данных
    IDatabaseModel *createInstance(QSqlDatabase db = QSqlDatabase::database());

};

#endif
