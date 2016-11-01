#ifndef IDATABASEMODELBUILDER_H
#define IDATABASEMODELBUILDER_H

#include <idatabaseclass.h>
#include <idatabaseattribute.h>
#include <idatabasefilter.h>
#include <idatabasecomposition.h>

//! Строитель модели базы данных
class IDatabaseModelBuilder
{

public:
    //! Создание экземпляра класса
    virtual IDatabaseClass *createClass() = 0;

    //! Создание экземпляра атрибута
    virtual IDatabaseAttribute *createAttr() = 0;

    //! Создание экземпляра фильтра
    virtual IDatabaseFilter *createFilter() = 0;

    //! Создание экземпляра состава
    virtual IDatabaseComposition *createComposition() = 0;

    //! Имя строителя модели базы данных
    virtual QString modelBuilderName() = 0;

};

#define IDatabaseModelBuilder_iid "com.RTPTechGroup.Raindrop.IDatabaseModelBuilder"
Q_DECLARE_INTERFACE(IDatabaseModelBuilder, IDatabaseModelBuilder_iid)

#endif // IDATABASEMODELBUILDER_H
