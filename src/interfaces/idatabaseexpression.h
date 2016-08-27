#ifndef IDATABASEEXPRESSION_H
#define IDATABASEEXPRESSION_H

#include <idatabaseattr.h>

#include <QString>
#include <QVariant>

class IDatabaseExpression;
class IDatabaseAttr;

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

    //! Логическое "ИЛИ"
    virtual IDatabaseExpression& operator || (const IDatabaseExpression &expr)  = 0;

    //! Логическое "И"
    virtual IDatabaseExpression& operator && (const IDatabaseExpression &expr)  = 0;

    //! Sql строка выражения
    virtual QString toSql() = 0;

    //! Устанавливает первое значение
    void setFirstValue(QVariant value) { m_firstValue = value; };

    //! Возращает первое значение
    QVariant firstValue() { return m_firstValue; };

    //! Устанавливает второе значение
    void setSecondValue(QVariant value) { m_secondValue = value; };

    //! Возращает второе значение
    QVariant secondValue() { return m_secondValue; };

    //! Устанавливает оператор
    void setExpressionOperator(ExpressionOperator oper) { m_operator = oper; };

    //! Возращает оператор
    ExpressionOperator expressionOperator()  { return m_operator; };
private:
    QVariant m_firstValue;
    QVariant m_secondValue;
    ExpressionOperator m_operator;

};
Q_DECLARE_METATYPE(IDatabaseExpression*)

#endif // IDATABASEEXPRESSION_H
