#ifndef IDATABASELIST_H
#define IDATABASELIST_H

#include <QtSql/QSqlQuery>

class IDatabaseClass;
class IDatabaseAttribute;
class IDatabaseFilter;
class IDatabaseComposition;
class IDatabaseObject;
template <class T1, class T2> class IDatabaseList;

typedef IDatabaseList<IDatabaseClass, IDatabaseClass>       IDatabaseClasses;
typedef IDatabaseList<IDatabaseClass, IDatabaseAttribute>   IDatabaseAttrs;
typedef IDatabaseList<IDatabaseClass, IDatabaseFilter>      IDatabaseFilters;
typedef IDatabaseList<IDatabaseClass, IDatabaseComposition> IDatabaseComps;

//! Список элементов базы данных
template <class T1, class T2>
class IDatabaseList
{
public:
    //! Конструктор класса
    explicit IDatabaseList(T1 *item)
    {
        m_item  = item;
        m_query = new QSqlQuery(item->database());
    }

    //! Деструктор класса
    virtual ~IDatabaseList()
    {
        delete m_query;
    }

    //! Получение первого элемента
    virtual bool first() { return m_query->first(); }

    //! Получение последнего элемента
    virtual bool last() { return m_query->last(); }

    //! Получение следующего элемента
    virtual bool next() { return m_query->next(); }

    //! Получение предыдущего элемента
    virtual bool previous() { return m_query->previous(); }

    //! Возращает элемент с позицией index
    virtual bool seek(int index, bool relative = false)
    { return m_query->seek(index, relative); }

    //! Получение элемента базы данных
    virtual T2 *value() = 0;

protected:
    T1 *m_item;
    QSqlQuery *m_query;
};

#endif // IDATABASELIST_H
