#ifndef IDATABASECOMPOSITION_H
#define IDATABASECOMPOSITION_H

#include <idatabaseitem.h>
#include <idatabaseattr.h>
#include <idatabaseexpression.h>


class IDatabaseAttr;

//! Состав класса базы данных
class IDatabaseComposition: public IDatabaseItem
{
public:
    //! Конструктор класса
    explicit IDatabaseComposition() {};

    //! Деструктор класса
    virtual ~IDatabaseComposition() {};

// Работа с выражениями
    //! Формирует выражение равенства
    IDatabaseExpression operator == (IDatabaseComposition &value)
    {
        return createCompExpression(value, IDatabaseExpression::EQ);
    }

    //! Формирует выражение равенства
    IDatabaseExpression operator == (IDatabaseAttr &value)
    {
        return createAttrExpression(value, IDatabaseExpression::EQ);
    }

    //! Формирует выражение неравенства
    IDatabaseExpression operator != (IDatabaseComposition &value)
    {
        return createCompExpression(value, IDatabaseExpression::NE);
    }

    //! Формирует выражение неравенства
    IDatabaseExpression operator != (IDatabaseAttr &value)
    {
        return createAttrExpression(value, IDatabaseExpression::NE);
    }

private:
    IDatabaseExpression createAttrExpression(
            IDatabaseAttr &value, IDatabaseExpression::ExpressionOperator oper)
    {
        IDatabaseExpression expr;
        IDatabaseComposition *firstValue
                = dynamic_cast<IDatabaseComposition *>(this);
        expr.setFirstValue(QVariant::fromValue(firstValue));
        expr.setSecondValue(QVariant::fromValue(&value));
        expr.setExpressionOperator(oper);
        return expr;
    }

    IDatabaseExpression createCompExpression(
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
