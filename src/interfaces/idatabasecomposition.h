#ifndef IDATABASECOMPOSITION_H
#define IDATABASECOMPOSITION_H

#include <idatabaseitem.h>
#include <idatabaseattribute.h>
#include <idatabaseexpression.h>


class IDatabaseAttribute;
class IDatabaseExpression;

//! Состав класса базы данных
class IDatabaseComposition: public IDatabaseItem
{
public:
    //! Конструктор класса
    explicit IDatabaseComposition() {}

    //! Деструктор класса
    virtual ~IDatabaseComposition() {}

// Работа с выражениями
    //! Формирует выражение равенства
    IDatabaseExpression operator == (IDatabaseComposition &value)
    {
        return createCompExpr(value, IDatabaseExpression::EQ);
    }

    //! Формирует выражение равенства
    IDatabaseExpression operator == (IDatabaseAttribute &value)
    {
        return createAttrExpr(value, IDatabaseExpression::EQ);
    }

    //! Формирует выражение неравенства
    IDatabaseExpression operator != (IDatabaseComposition &value)
    {
        return createCompExpr(value, IDatabaseExpression::NE);
    }

    //! Формирует выражение неравенства
    IDatabaseExpression operator != (IDatabaseAttribute &value)
    {
        return createAttrExpr(value, IDatabaseExpression::NE);
    }

private:
    IDatabaseExpression createAttrExpr(
            IDatabaseAttribute &value, IDatabaseExpression::ExpressionOperator oper)
    {
        IDatabaseExpression expr;
        IDatabaseComposition *firstValue
                = dynamic_cast<IDatabaseComposition *>(this);
        expr.setFirstValue(QVariant::fromValue(firstValue));
        expr.setSecondValue(QVariant::fromValue(&value));
        expr.setExpressionOperator(oper);
        return expr;
    }

    IDatabaseExpression createCompExpr(
            IDatabaseComposition &value, IDatabaseExpression::ExpressionOperator oper)
    {
        IDatabaseExpression expr;
        IDatabaseComposition *firstValue
                = dynamic_cast<IDatabaseComposition *>(this);
        expr.setFirstValue(QVariant::fromValue(firstValue));
        expr.setSecondValue(QVariant::fromValue(&value));
        expr.setExpressionOperator(oper);
        return expr;
    }
};

Q_DECLARE_METATYPE(IDatabaseComposition*)

#endif
