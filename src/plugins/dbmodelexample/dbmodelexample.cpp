#include "dbmodelexample.h"

#include <plugin/pluginmanager.h>
#include <idatabasemodel.h>
#include <idatabasemodelmanager.h>

#include <metadatamodel/dbxmlstruct.h>

#include "clogging.h"

using namespace RTPTechGroup::Plugin;
using namespace RTPTechGroup::MetaDataModel;

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
    /*
    IDatabaseClass *cls = dbModel->createDerivedClass(DBCLASSXML::CLASS);
    cls = dbModel->createDerivedClass(DBLOVXML::LOV);
    cls = dbModel->createDerivedClass(DBQUANTITYGROUPXML::QUANTITYGROUP);
    cls = dbModel->createDerivedClass(DBQUANTITYXML::QUANTITY);
    cls = dbModel->createDerivedClass(DBUNITXML::UNIT);
    cls = dbModel->createDerivedClass(DBQUANTITYXML::QUANTITY);
    cls = dbModel->createDerivedClass(DBNUMERATORXML::NUMERATOR);
    cls = dbModel->createDerivedClass(DBNUMERATORLOVXML::NUMERATORLOV);
    cls = dbModel->createDerivedClass(DBNUMERATORREGEXXML::NUMERATORREGEX);
    cls = dbModel->createDerivedClass(DBATTRXML::ATTR);
    cls = dbModel->createDerivedClass(DBCOMPXML::COMP);
    cls = dbModel->createDerivedClass(DBFILTERXML::FILTER);
    cls = dbModel->createDerivedClass(DBFILTERBLOCKXML::BLOCK);
    cls = dbModel->createDerivedClass(DBCONDITIONXML::COND);
    cls = dbModel->createDerivedClass(DBREFGROUPXML::REFGROUP);
    cls = dbModel->createDerivedClass(DBREFXML::REF);
    cls = dbModel->createDerivedClass(DBLINKTOFILTERXML::LINKTOFILTER);
    cls = dbModel->createDerivedClass(DBLINKTOREFXML::LINKTOREF);
    cls = dbModel->createDerivedClass(DBLINKTOCLASSXML::LINKTOCLASS);
    cls = dbModel->createDerivedClass(DBLINKTOFILTERXML::LINKTOFILTER);
    cls = dbModel->createDerivedClass(DBROLEXML::ROLE);
    cls = dbModel->createDerivedClass(DBPERMISSIONXML::PERMISSION);
    */
    dbModel->init();

    if (dbModel) {
        IDatabaseClass *dbClass = dbModel->oneClass("TestClass");
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
        IDatabaseClass *dbNewClass = dbModel->createClass("TestNewClass");
        if (dbNewClass) {
            dbNewClass->setAlias("Тестовый класс");
            dbNewClass->setMaxVersion(5);
            dbNewClass->push();
        }
    }
}

}}

