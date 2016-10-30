#ifndef IDATABASEMODELBUILDER_H
#define IDATABASEMODELBUILDER_H

#include <idatabaseclass.h>
#include <idatabaseattr.h>
#include <idatabasefilter.h>
#include <idatabasecomposition.h>

//! Строитель модели базы данных
class IDatabaseModelBuilder
{

public:
    //! Создание экземпляра класса
    virtual IDatabaseClass *createClass() = 0;

    //! Создание экземпляра атрибута
    virtual IDatabaseAttr *createAttr() = 0;

    //! Создание экземпляра фильтра
    virtual IDatabaseFilter *createFilter() = 0;

    //! Создание экземпляра состава
    virtual IDatabaseComposition *createComposition() = 0;

    //! Имя строителя модели базы данных
    virtual QString modelBuilderName() = 0;

};

Q_DECLARE_INTERFACE(IDatabaseModelBuilder,"com.RTPTechGroup.Raindrop.IDatabaseModelBuilder/1.0")

#endif // IDATABASEMODELBUILDER_H
