#ifndef IDATABASEMODEL_H
#define IDATABASEMODEL_H

#include <idatabaseclass.h>

//! Модель базы данных
class IDatabaseModel
{
public:
    //! Конструктор класса
    explicit IDatabaseModel() {}

    //! Деструктор класса
    virtual ~IDatabaseModel() {}

// Создание элемента по имени
    //! Создание производного класса по имени
    virtual IDatabaseClass *createModelClass(const QString &name) = 0;

// Получение доступа к элементам модели по имени
    //! Получение производного класса по имени
    virtual IDatabaseClass *modelClass(const QString &name) = 0;

// Получение доступа к спискам элементов модели
    //! Cписок всех классов
    virtual IDatabaseClasses *classList() = 0;

    //! Cписок классов модели
    virtual IDatabaseClasses *modelClassList() = 0;

};

#endif // IDATABASEMODEL_H
