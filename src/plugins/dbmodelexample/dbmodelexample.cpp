#include "dbmodelexample.h"

#include <plugin/pluginmanager.h>
#include <databasemodel/idatabasemodel.h>
#include <databasemodel/idatabaseerror.h>
#include <databasemodel/idatabasemodelmanager.h>

#include <metadatamodel/dbxmlstruct.h>

#include "clogging.h"

using namespace RTPTechGroup::Plugin;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace DatabaseModel {

DbModelExample::DbModelExample(QObject *parent):
    QObject(parent), IPlugin("")
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("customdb");
    db.setUserName("postgres");
    db.setPassword("");
    bool ok = db.open();
    if (!ok)
        return;
    qCWarning(lcDbModelExample) << tr("Соединение с базой данных: ") << ok;

    // Текущий класс
    IDatabaseModelManager *dbModelManager = IDatabaseModelManager::instance();
    IDatabaseModel *dbModel = dbModelManager->createModel(db);
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
//    dbModel->init();

    if (dbModel) {
        IDatabaseClass *dbClass = dbModel->oneClass("TestClass");
        if (dbClass) {
            dbClass->pull();
            int count = dbClass->count();

            // Создание атрибута
            IDatabaseAttribute *dbAttr = dbClass->attr("name");
            if (dbAttr != nullptr) {
                dbAttr->setAlias("Атрибут");
                dbAttr->setInitialValue(0);
                dbAttr->create();

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
                    delete object;
                    delete objects;
                }
                delete dbAttr;
            }
            delete dbClass;
        }

        // Создание класса               
        IDatabaseSession *dbSession = dbModel->createSession();

        IDatabaseClass *dbNewClass = dbModel->oneClass("TestNewClass");
        if (dbNewClass != nullptr) {
            dbNewClass->setAlias("Тестовый класс");
            dbNewClass->setMaxVersion(5);

            dbSession->transaction();
            IDatabaseError err = dbNewClass->create(dbSession);

            err.setHandlerDone([](IDatabaseError &error){
                error.session()->commit();
            });

            QThread::sleep(5);
            qApp->processEvents();
            qDebug() << err.sqlError().text();

            IDatabaseAttribute *dbNewAttr = dbNewClass->attr("name");
            if (dbNewAttr != nullptr) {
                dbNewAttr->setAlias("Атрибут");
                dbNewAttr->setInitialValue(0);
                dbNewAttr->create(dbSession);
                delete dbNewAttr;
            }
            delete dbNewClass;
        }
        delete dbSession;
        delete dbModel;
    }
}

}}

