#ifndef IDATABASEMODEL_H
#define IDATABASEMODEL_H

#include <idatabasemodelbuilder.h>

//! Модель базы данных
class IDatabaseModel
{
    //! Получение строителя по имени
    virtual IDatabaseModelBuilder *modelBuilder(const QString& name) = 0;
};

Q_DECLARE_INTERFACE(IDatabaseModel,"com.RTPTechGroup.Raindrop.IDatabaseModel/1.0")

#endif
