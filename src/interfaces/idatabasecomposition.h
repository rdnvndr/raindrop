#ifndef IDATABASECOMPOSITION_H
#define IDATABASECOMPOSITION_H

#include <idatabaseitem.h>
#include <idatabaseclass.h>
#include <idatabaseattribute.h>
#include <idatabaseexpression.h>

class IDatabaseClass;
class IDatabaseAttribute;
class IDatabaseExpression;

//! Состав класса базы данных
class IDatabaseComposition: public IDatabaseItem
{
public:

    //! Деструктор класса
    virtual ~IDatabaseComposition() {}

    //! Устанавливает основной класс
    virtual void setPrimaryClass(IDatabaseClass *cls) {
        m_primaryClass = cls;
    }

    //!  Возвращает второй класс
    virtual IDatabaseClass *secondClass() const {
        return m_secondClass;
    }

     //! Устанавливает второй класс
    virtual void setSecondClass(IDatabaseClass *cls) {
        m_secondClass = cls;
    }

    //! Возвращает описание прямой связи
    virtual QString directDescrition() const {
        return m_directDescrition;
    }

    //! Устанавливает описание прямой связи
    virtual void setDirectDescrition(const QString &directDescrition) {
       m_directDescrition = directDescrition;
    }

    //! Возвращает описание обратной связи
    virtual QString inverseDescrition() const {
        return m_inverseDescrition;
    }

    //! Устанавливает описание обратной связи
    virtual void setInverseDescrition(const QString &inverseDescrition) {
        m_inverseDescrition = inverseDescrition;
    }

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

private:
    //! Основной класс
    IDatabaseClass *m_primaryClass;

    //! Второй класс
    IDatabaseClass *m_secondClass;

    //! Описание прямой связи
    QString m_directDescrition;

    //! Описание обратной связи
    QString m_inverseDescrition;
};

Q_DECLARE_METATYPE(IDatabaseComposition*)

#endif
