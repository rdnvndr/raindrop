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
    virtual IDatabaseClass *oneClass(const QString &name) = 0;

// Получение доступа к спискам элементов модели
    //! Список всех классов модели
    virtual IDatabaseClasses *classList() = 0;

    //! Список производных классов модели
    virtual IDatabaseClasses *derivedClassList(
            const QString &base) = 0;

private:
    //! Пул SQL запросов
    IDatabasePool *m_pool;
};

#endif // IDATABASEMODEL_H
