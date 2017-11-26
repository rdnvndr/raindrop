#include "dbmodelexample.h"

#include <plugin/pluginmanager.h>
#include <idatabasemodel.h>
#include <idatabasemodelmanager.h>

#include "clogging.h"

using namespace RTPTechGroup::Plugin;

namespace RTPTechGroup {
namespace DatabaseModel {

DbModelExample::DbModelExample(QObject *parent):
    QObject(parent), IPlugin("IDatabaseModelManager")
{
    PluginManager *pluginManager = PluginManager::instance();
    IDatabaseModelManager *dbModelManager
            = pluginManager->interfaceObject<IDatabaseModelManager *>(
                "IDatabaseModelManager");


    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("customdb");
    db.setUserName("postgres");
    db.setPassword("");
    bool ok = db.open();
    qCWarning(lcDbModelExample) << tr("Соединение с базой данных: ") << ok;

    // Текущий класс
    IDatabaseModel *dbModel = dbModelManager->createInstance(db);
    dbModel->init();

    if (dbModel) {
        IDatabaseClass *dbClass = dbModel->derivedClass("TestClass");
        if (dbClass) {
            dbClass->pull();
            int count = dbClass->count();

            // Создание атрибута
            IDatabaseAttribute *dbAttr = dbClass->createAttr("name");
            if (dbAttr) {
                dbAttr->setAlias("Атрибут");
                dbAttr->setInitialValue(0);
                dbAttr->push();

                // Получение объектов
                IDatabaseObjects *objects
                        = dbClass->values()->orderByAsc(dbAttr);
                if (objects) {
                    objects->exec();
                    objects->next();
                    IDatabaseObject *object = objects->value();
//                    QVariant value = object->value(dbAttr);
//                    object->value(dbAttr) = "test";
//                    object->push();
                }
            }
        }

        // Создание класса
        IDatabaseClass *dbNewClass = dbModel->createDerivedClass("TestNewClass");
        if (dbNewClass) {
            dbNewClass->setAlias("Тестовый класс");
            dbNewClass->setMaxVersion(5);
            dbNewClass->push();
        }
    }
}

}}

