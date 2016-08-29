#include "pgdatabaseexpression.h"

PgDatabaseExpression::PgDatabaseExpression()
{

}

PgDatabaseExpression::~PgDatabaseExpression()
{

}

IDatabaseExpression &PgDatabaseExpression::operator ||(IDatabaseExpression &expr)
{
    return createExpression(expr, IDatabaseExpression::OR);
}

IDatabaseExpression &PgDatabaseExpression::operator &&(IDatabaseExpression &expr)
{
    return createExpression(expr, IDatabaseExpression::AND);
}

QString PgDatabaseExpression::toSql()
{
    return QString();
}

IDatabaseExpression &PgDatabaseExpression::createExpression(
        IDatabaseExpression &expr, IDatabaseExpression::ExpressionOperator oper)
{
    PgDatabaseExpression *returnExpr = new PgDatabaseExpression();
    IDatabaseExpression *firstValue = dynamic_cast<IDatabaseExpression *>(this);
    returnExpr->setFirstValue(QVariant::fromValue(firstValue));
    returnExpr->setSecondValue(QVariant::fromValue(&expr));
    returnExpr->setExpressionOperator(oper);
    return *returnExpr;
}
