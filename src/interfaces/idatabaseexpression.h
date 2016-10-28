#ifndef IDATABASEEXPRESSION_H
#define IDATABASEEXPRESSION_H

#include <idatabaseattr.h>

#include <QString>
#include <QVariant>



//! Выражение базы данных
class IDatabaseExpression
{
public:
    //! Операторы выражений  базы данных
    enum ExpressionOperator {  EQ,   //! Равно
                               NE,   //! Не равно
                               GT,   //! Больше
                               GE,   //! Больше или равно
                               LT,   //! Меньше
                               LE,   //! Меньше или равно
                               LIKE, //! Похоже
                               OR,   //! Логическое ИЛИ
                               AND   //! Логическое И
                             };

    //! Конструктор класса
    explicit IDatabaseExpression() {};

    //! Деструктор класса
    virtual ~IDatabaseExpression() {};

    //! Устанавливает первое значение
    void setFirstValue(QVariant value) { m_firstValue = value; }

    //! Возращает первое значение
    QVariant firstValue() { return m_firstValue; }

    //! Устанавливает второе значение
    void setSecondValue(QVariant value) { m_secondValue = value; }

    //! Возращает второе значение
    QVariant secondValue() { return m_secondValue; }

    //! Устанавливает оператор
    void setExpressionOperator(ExpressionOperator oper) { m_operator = oper; }

    //! Возращает оператор
    ExpressionOperator expressionOperator()  { return m_operator; }

    //! Логическое "ИЛИ"
    IDatabaseExpression operator ||(const IDatabaseExpression &expr)
    {
        return createExpression(expr, IDatabaseExpression::OR);
    }

    //! Логическое "И"
    IDatabaseExpression operator &&(const IDatabaseExpression &expr)
    {
        return createExpression(expr, IDatabaseExpression::AND);
    }

    IDatabaseExpression createExpression(
            const IDatabaseExpression &expr, IDatabaseExpression::ExpressionOperator oper)
    {
        IDatabaseExpression returnExpr;
        returnExpr.setFirstValue(QVariant::fromValue(*this));
        returnExpr.setSecondValue(QVariant::fromValue(expr));
        returnExpr.setExpressionOperator(oper);
        return returnExpr;
    }

private:
    QVariant m_firstValue;
    QVariant m_secondValue;
    ExpressionOperator m_operator =  IDatabaseExpression::EQ;
};

Q_DECLARE_METATYPE(IDatabaseExpression)

#endif // IDATABASEEXPRESSION_H
