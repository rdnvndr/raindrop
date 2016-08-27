#ifndef PGDATABASEEXPRESSION_H
#define PGDATABASEEXPRESSION_H

#include <idatabaseexpression.h>

class PgDatabaseExpression : public IDatabaseExpression
{
public:
    //! Конструктор класса
    explicit PgDatabaseExpression();

    //! Деструктор класса
    virtual ~PgDatabaseExpression();

    //! Логическое "ИЛИ"
    IDatabaseExpression &operator || (const IDatabaseExpression &expr);

    //! Логическое "И"
    IDatabaseExpression &operator && (const IDatabaseExpression &expr);

    //! Sql строка выражения
    virtual QString toSql();

};

#endif // PGDATABASEEXPRESSION_H
