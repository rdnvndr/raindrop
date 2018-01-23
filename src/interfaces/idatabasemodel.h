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

    //! Инициализирует модель базы данных
    virtual bool init() = 0;

// Создание элемента по имени
    //! Создание класса по имени
    virtual IDatabaseClass *createClass(
            const QString &name, const QString &base = QString("")) = 0;

// Получение доступа к элементам модели по имени
    //! Получение класса по имени
    virtual IDatabaseClass *oneClass(
            const QString &name, const QString &base = QString("")) = 0;

// Получение доступа к спискам элементов модели
    //! Cписок всех классов модели
    virtual IDatabaseClasses *classList() = 0;

    //! Cписок классов модели
    virtual IDatabaseClasses *classList(
            const QString &base) = 0;

};

#endif // IDATABASEMODEL_H
