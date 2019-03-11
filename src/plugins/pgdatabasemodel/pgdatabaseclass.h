#ifndef PGDATABASECLASS_H
#define PGDATABASECLASS_H

#include <idatabaseclass.h>

namespace RTPTechGroup {
namespace DatabaseModel {

//! Реализация класса базы данных PostgreSql
class PgDatabaseClass : public QObject, public IDatabaseClass
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit PgDatabaseClass(const QString &name, IDatabasePool *pool);

    //! Деструктор класса
    virtual ~PgDatabaseClass();

// Работа с элементами базы данных
    //! Создаёт элемент базы данных
    virtual void create(IDatabaseThread *databaseThread = nullptr);

    //! Отправить изменения элемента базы данных
    virtual void push(IDatabaseThread *databaseThread = nullptr);

    //! Получить изменения элемента базы данных
    virtual void pull(IDatabaseThread *databaseThread = nullptr);

    //! Удаление элемента базы данных
    virtual void remove(IDatabaseThread *databaseThread = nullptr);

// Получение доступа к элементам класса по имени
    //! Получение производного класса по имени
    IDatabaseClass *derivedClass(const QString &name) ;

    //! Получение атрибута по имени
    IDatabaseAttribute *attr(const QString &name);

    //! Получение фильтра по имени
    IDatabaseFilter *filter(const QString &name);

    //! Получение состава по имени
    IDatabaseComposition *comp(const QString &name);

// Получение доступа к спискам элементов класса по имени
    //! Список производных классов
    IDatabaseClasses *derivedClassList();

    //! Список атрибутов в классе
    IDatabaseAttrs *attrList();

    //! Список фильтров в классе
    IDatabaseFilters *filterList();

    //! Список атрибутов в классе
    IDatabaseComps *compList();

// Работа с объектами
    //! Получение отфильтрованных объектов класса
    virtual IDatabaseObjects *filtering(IDatabaseFilter *filter);

    //! Получение отфильтрованных объектов класса
    virtual IDatabaseObjects *filtering(IDatabaseExpression *expr);


    //! Получение отсортированных объектов по возрастанию
    virtual IDatabaseObjects *orderByAsc(IDatabaseAttribute *attr, ...);

    //! Получение отсортированных объектов по убыванию
    virtual IDatabaseObjects *orderByDesc(IDatabaseAttribute *attr, ...);

    //! Получение уникальных объектов
    virtual IDatabaseObjects *distinct(IDatabaseAttribute *attr, ...);

    //! Ограничение объектов
    virtual IDatabaseObjects *limit(int up, int down);

    //! Ограничение списка возвращаемых полей
    virtual IDatabaseObjects *values(IDatabaseAttribute *attr = nullptr, ...);


    //! Сцепление объектов
    virtual IDatabaseObjects *join(IDatabaseFilter *filter,
                                   JoinType joinType = Inner);

    //! Сцепление объектов
    virtual IDatabaseObjects *join(IDatabaseExpression *expr,
                                   JoinType joinType = Inner);

    //! Объединение объектов
    virtual IDatabaseObjects *unionAll(IDatabaseObjects *objs);


    //! Получение количества объектов
    virtual int count(IDatabaseAttribute *attr = nullptr, bool distinct = false);

    //! Получение среднего значения атрибута
    virtual int avg(IDatabaseAttribute *attr);

    //! Получение минимального значения атрибута
    virtual int min(IDatabaseAttribute *attr);

    //! Получение максимального значения атрибута
    virtual int max(IDatabaseAttribute *attr);

    //! Получение суммарного значения атрибута
    virtual int sum(IDatabaseAttribute *attr);

signals:
    //! Сигнал об окончании выполнения операции в потоке
    void done();

    //! Сигнал об ошибке в потоке
    void error(QSqlError err);

};

}}

#endif // PGDATABASECLASS_H
