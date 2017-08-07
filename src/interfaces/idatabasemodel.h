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
    virtual IDatabaseClass *createDerivedClass(
            const QString &name, const QString &base = QString("")) = 0;

// Получение доступа к элементам модели по имени
    //! Получение производного класса по имени
    virtual IDatabaseClass *derivedClass(
            const QString &name, const QString &base = QString("")) = 0;

// Получение доступа к спискам элементов модели
    //! Cписок всех классов
    virtual IDatabaseClasses *allClassList() = 0;

    //! Cписок производных классов модели
    virtual IDatabaseClasses *derivedClassList(
            const QString &base = QString("")) = 0;

};

#endif // IDATABASEMODEL_H
