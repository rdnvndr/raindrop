#ifndef IDATABASEMODEL_H
#define IDATABASEMODEL_H

#include <idatabasemodelbuilder.h>

//! Модель базы данных
class IDatabaseModel
{
    //! Получение строителя по имени
    virtual IDatabaseModelBuilder *modelBuilder(const QString& name) = 0;
};
#define IDatabaseModel_iid "com.RTPTechGroup.Raindrop.IDatabaseModel"
Q_DECLARE_INTERFACE(IDatabaseModel, IDatabaseModel_iid)

#endif
