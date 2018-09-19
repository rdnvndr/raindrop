#ifndef IDATABASEMODELMANAGER_H
#define IDATABASEMODELMANAGER_H

#include <idatabasemodelbuilder.h>

//! Менеджер модели базы данных
class IDatabaseModelManager
{

public:
    //! Конструктор класса
    explicit IDatabaseModelManager() {

    }

    //! Деструктор класса
    virtual ~IDatabaseModelManager() {}

// IDatabaseModel
    //! Получение модели базы данных
    virtual IDatabaseModel *createInstance(
            QSqlDatabase db = QSqlDatabase::database()) = 0;
};

#define IDatabaseModelManager_iid "com.RTPTechGroup.Raindrop.IDatabaseModelManager"
Q_DECLARE_INTERFACE(IDatabaseModelManager, IDatabaseModelManager_iid)

#endif
