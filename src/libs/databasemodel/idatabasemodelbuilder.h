#ifndef IDATABASEMODELBUILDER_H
#define IDATABASEMODELBUILDER_H

#include <databasemodel/idatabasemodel.h>
#include "databasemodelglobal.h"

//! Строитель модели базы данных
class DATABASEMODELLIB IDatabaseModelBuilder
{

public:
    //! Создание экземпляра модели базы данных
    virtual IDatabaseModel *createModel(IDatabasePool *pool) = 0;

    //! Имя драйвера базы данных для которого реализована модель
    virtual QString implDriverName() const = 0;

};

#define IDatabaseModelBuilder_iid "com.RTPTechGroup.Raindrop.IDatabaseModelBuilder"
Q_DECLARE_INTERFACE(IDatabaseModelBuilder, IDatabaseModelBuilder_iid)

#endif // IDATABASEMODELBUILDER_H
