#ifndef IDATABASEMODELMANAGER_H
#define IDATABASEMODELMANAGER_H

#include <databasemodel/idatabasemodel.h>
#include "databasemodelglobal.h"

//! Менеджер модели базы данных
class DATABASEMODELLIB IDatabaseModelManager
{

public:
    //! Получение экземпляра менеджера
    static IDatabaseModelManager *instance();

    //! Конструктор класса
    explicit IDatabaseModelManager();

    //! Деструктор класса
    virtual ~IDatabaseModelManager();

// IDatabaseModel
    //! Создание модели базы данных
    virtual IDatabaseModel *createDatabaseModel(QSqlDatabase db = QSqlDatabase::database());

private:
    //! Экземпляр менеджера
    static IDatabaseModelManager *m_instance;
};

#endif
