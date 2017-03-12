#ifndef IDATABASEMODELBUILDER_H
#define IDATABASEMODELBUILDER_H

#include <QtSql/QSqlDatabase>

#include <idatabasemodel.h>

//! Строитель модели базы данных
class IDatabaseModelBuilder
{

public:
    //! Создание экземпляра модели базы данных
    virtual IDatabaseModel *createDatabaseModel(QSqlDatabase db) = 0;

    //! Имя драйвера базы данных для которого реализована модель
    virtual QString implDriverName() const = 0;

};

#define IDatabaseModelBuilder_iid "com.RTPTechGroup.Raindrop.IDatabaseModelBuilder"
Q_DECLARE_INTERFACE(IDatabaseModelBuilder, IDatabaseModelBuilder_iid)

#endif // IDATABASEMODELBUILDER_H
