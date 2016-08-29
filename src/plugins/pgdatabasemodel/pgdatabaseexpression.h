#ifndef PGDATABASEEXPRESSION_H
#define PGDATABASEEXPRESSION_H

#include <idatabaseexpression.h>

//! Реализация выражение базы данных
class PgDatabaseExpression : public IDatabaseExpression
{
public:
    //! Конструктор класса
    explicit PgDatabaseExpression();

    //! Деструктор класса
    virtual ~PgDatabaseExpression();

    //! Логическое "ИЛИ"
    IDatabaseExpression &operator || (IDatabaseExpression &expr);

    //! Логическое "И"
    IDatabaseExpression &operator && (IDatabaseExpression &expr);

    //! Sql строка выражения
    virtual QString toSql();

private:
    //! Формирование выражения для указанного логического оператора
    IDatabaseExpression& createExpression(
            IDatabaseExpression &expr, IDatabaseExpression::ExpressionOperator oper);
};

#endif // PGDATABASEEXPRESSION_H
