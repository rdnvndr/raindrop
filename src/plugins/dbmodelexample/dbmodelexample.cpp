#include "dbmodelexample.h"

#include <plugin/pluginmanager.h>
#include <idatabasemodel.h>
#include <idatabasemodelmanager.h>

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

    // Текущий класс
    IDatabaseModel *dbModel = dbModelManager->createInstance();
    IDatabaseClass *dbClass = dbModel->modelClass("TestClass");
    dbClass->pull();

    // Создание атрибута
    IDatabaseAttribute *dbAttr = dbClass->createAttr("name");
    dbAttr->setAlias("Атрибут");
    dbAttr->setInitialValue(0);
    dbAttr->push();

    // Получение объектов


    // Создание класса
    IDatabaseClass *dbNewClass = dbModel->createModelClass("TestNewClass");
    dbNewClass->setAlias("Тестовый класс");
    dbNewClass->setMaxVersion(5);
    dbNewClass->push();

}

}}

