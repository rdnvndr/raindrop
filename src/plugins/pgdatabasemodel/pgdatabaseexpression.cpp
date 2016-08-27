#include "pgdatabaseexpression.h"

PgDatabaseExpression::PgDatabaseExpression()
{

}

PgDatabaseExpression::~PgDatabaseExpression()
{

}

IDatabaseExpression &PgDatabaseExpression::operator ||(const IDatabaseExpression &expr)
{
    PgDatabaseExpression *returnExpr = new PgDatabaseExpression();
//    IDatabaseExpression *firstValue = dynamic_cast<IDatabaseExpression *>(this);
//    returnExpr->setFirstValue(QVariant::fromValue(firstValue));
//    returnExpr->setSecondValue(QVariant::fromValue(expr));
//    returnExpr->setExpressionOperator(IDatabaseExpression::OR);
//    return returnExpr;
    return *returnExpr;
}

IDatabaseExpression &PgDatabaseExpression::operator &&(const IDatabaseExpression &expr)
{
    PgDatabaseExpression *returnExpr = new PgDatabaseExpression();
//    IDatabaseExpression *firstValue = dynamic_cast<IDatabaseExpression *>(this);
//    returnExpr->setFirstValue(QVariant::fromValue(firstValue));
//    returnExpr->setSecondValue(QVariant::fromValue(expr));
//    returnExpr->setExpressionOperator(IDatabaseExpression::AND);
//    return returnExpr;
    return *returnExpr;
}

QString PgDatabaseExpression::toSql()
{
    return QString();
}
