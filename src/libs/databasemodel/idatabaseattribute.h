#ifndef IDATABASEATTRIBUTE_H
#define IDATABASEATTRIBUTE_H

#include <databasemodel/idatabaseexpression.h>
#include <databasemodel/idatabaseitem.h>
#include <databasemodel/idatabaseclass.h>
#include <databasemodel/idatabasecomposition.h>
#include "databasemodelglobal.h"

#include <QVariant>
#include <QUuid>


class IDatabaseClass;
class IDatabaseComposition;

//! Атрибут класса базы данных
class DATABASEMODELLIB IDatabaseAttribute: public IDatabaseItem
{
public:
    //! Перечисление типов атрибутов
    enum AttrbuteType { Boolean,   //!< Логический
                        Binary,    //!< Бинарный
                        Char,      //!< Символ
                        Date,      //!< Дата
                        Decimal,   //!< Число с фиксированной точностью
                        Double,    //!< Вещественный
                        Integer,   //!< Целый
                        String,    //!< Строка
                        Reference, //!< Ссылка
                        Time,      //!< Время
                        Timeshtamp //!< Отметка времени
                      };

    //! Деструктор класса
    virtual ~IDatabaseAttribute() {}

    //! Возвращает тип атрибута
    virtual AttrbuteType attributeType() { return m_attrType; }

    //! Устанавливает тип атрибута
    virtual void setAttrbuteType(AttrbuteType attrType) {
        m_sync = false;
        m_attrType = attrType;
    }

     //! Возвращает группу атрибута
    virtual QString group() const { return m_group; }

    //! Устанавливает группу атрибута
    virtual void setGroup(const QString &group) {
        m_sync = false;
        m_group = group;
    }

     //! Возвращает максимальную длину
    virtual qint32 maxStringLength() { return m_length; }

    //! Устанавливает максимальную длину
    virtual void setMaxStringLength(qint32 length) {
        m_sync = false;
        m_length = length;
    }

     //! Возвращает точность
    virtual qint32 accuracy() { return m_accuracy; }

    //! Устанавливает точность
    virtual void setAccuracy(qint32 accuracy) {
        m_sync = false;
        m_accuracy = accuracy;
    }

     //! Возвращает разрешение присваивать NULL
    virtual bool isNullsAllowed() { return m_nullsAllowed; }

    //! Устанавливает разрешение присваивать NULL
    virtual void setNullsAllowed(bool nullsAllowed) {
        m_sync = false;
        m_nullsAllowed = nullsAllowed;
    }

     //! Возвращает уникальность атрибута
    virtual bool isUnique() { return m_unique; }

    //! Устанавливает уникальность атрибута
    virtual void setUnique(bool unique) {
        m_sync = false;
        m_unique = unique;
    }

     //! Возвращает является ли атрибут ключом
    virtual bool isCandidateKey() { return m_candidateKey; }

    //! Устанавливает атрибут ключом
    virtual void setCandidateKey(bool candidateKey) {
        m_sync = false;
        m_candidateKey = candidateKey;
    }

     //! Возвращает значение по умолчанию
    virtual QVariant initialValue() { return m_initialValue; }

    //! Устанавливает значение по умолчанию
    virtual void setInitialValue(QVariant initialValue) {
        m_sync = false;
        m_initialValue = initialValue;
    }

     //! Возвращает нижнюю границу атрибута
    virtual QVariant lowerBound() { return m_lowerBound; }

    //! Устанавливает нижнюю границу атрибута
    virtual void setLowerBound(QVariant lowerBound) {
        m_sync = false;
        m_lowerBound = lowerBound;
    }

     //! Возвращает верхнюю границу атрибута
    virtual QVariant upperBound() { return m_upperBound; }

    //! Устанавливает верхнюю границу атрибута
    virtual void setUpperBound(QVariant upperBound) {
        m_sync = false;
        m_upperBound = upperBound;
    }

     //! Возвращает ссылочный класс
    virtual IDatabaseClass *referencedClass() { return m_refClass; }

    //! Устанавливает ссылочный класс
    virtual void setReferencedClass(IDatabaseClass *refClass) {
        m_sync = false;
        m_refClass = refClass;
    }

     //! Возвращает единицу измерения атрибута
    virtual IDatabaseClass *referencedUnit() { return m_refUnit; }

    //! Устанавливает единицу измерения атрибута
    virtual void setReferencedUnit(IDatabaseClass *refUnit) {
        m_sync = false;
        m_refUnit = refUnit;
    }

     //! Возвращает список значений атрибута
    virtual IDatabaseClass *referencedLov() { return m_refLov; }

    //! Устанавливает список значений атрибута
    virtual void setReferencedLov(IDatabaseClass *refLov) {
        m_sync = false;
        m_refLov = refLov;
    }

     //! Возвращает нумератор для атрибута
    virtual IDatabaseClass *referenceNumerator() { return m_refNumerator; }

    //! Устанавливает нумератор для атрибута
    virtual void setReferenceNumerator(IDatabaseClass *refNumerator) {
        m_sync = false;
        m_refNumerator = refNumerator;
    }

     //! Возвращает класс атрибута
    virtual IDatabaseClass *parent() { return m_parent; }

    //! Устанавливает класс атрибута
    virtual void setParent(IDatabaseClass *parent) {
        m_sync = false;
        m_parent = parent;
    }

    //! Проверка наследования атрибута
    virtual bool isInherited(IDatabaseClass *parent) = 0;

// Работа с выражениями
    //! Формирует выражение равенства
    IDatabaseExpression operator == (QVariant value)
    {
        return createVariantExpr(value, IDatabaseExpression::EQ);
    }

    //! Формирует выражение равенства
    IDatabaseExpression operator == (IDatabaseAttribute &value)
    {
        return createAttrExpr(value, IDatabaseExpression::EQ);
    }

    //! Формирует выражение равенства
    IDatabaseExpression operator == (IDatabaseComposition &value)
    {
        return createCompExpr(value, IDatabaseExpression::EQ);
    }

    //! Формирует выражение неравенства
    IDatabaseExpression operator != (QVariant value)
    {
        return createVariantExpr(value, IDatabaseExpression::NE);
    }

    //! Формирует выражение неравенства
    IDatabaseExpression operator != (IDatabaseAttribute &value)
    {
        return createAttrExpr(value, IDatabaseExpression::NE);
    }

    //! Формирует выражение неравенства
    IDatabaseExpression operator != (IDatabaseComposition &value)
    {
        return createCompExpr(value, IDatabaseExpression::NE);
    }

    //! Формирует выражение больше или равно
    IDatabaseExpression operator >= (QVariant value)
    {
        return createVariantExpr(value, IDatabaseExpression::GE);
    }

    //! Формирует выражение больше или равно
    IDatabaseExpression operator >= (IDatabaseAttribute &value)
    {
        return createAttrExpr(value, IDatabaseExpression::GE);
    }

    //! Формирует выражение меньше или равно
    IDatabaseExpression operator <= (QVariant value)
    {
        return createVariantExpr(value, IDatabaseExpression::LE);
    }

    //! Формирует выражение меньше или равно
    IDatabaseExpression operator <= (IDatabaseAttribute &value)
    {
        return createAttrExpr(value, IDatabaseExpression::LE);
    }

    //! Формирует выражение больше
    IDatabaseExpression operator >  (QVariant value)
    {
        return createVariantExpr(value, IDatabaseExpression::GT);
    }

    //! Формирует выражение больше
    IDatabaseExpression operator >  (IDatabaseAttribute &value)
    {
        return createAttrExpr(value, IDatabaseExpression::GT);
    }

    //! Формирует выражение меньше
    IDatabaseExpression operator <  (QVariant value)
    {
        return createVariantExpr(value, IDatabaseExpression::LT);
    }

    //! Формирует выражение меньше
    IDatabaseExpression operator <  (IDatabaseAttribute &value)
    {
        return createAttrExpr(value, IDatabaseExpression::LT);
    }

    //! Формирует выражение содержит
    IDatabaseExpression contains (QVariant value)
    {
        return createVariantExpr(value, IDatabaseExpression::LIKE);
    }

   //! Формирует выражение содержит
    IDatabaseExpression contains (IDatabaseAttribute &value)
    {
        return createAttrExpr(value, IDatabaseExpression::LIKE);
    }

protected:
    //! Конструктор класса
    explicit IDatabaseAttribute(IDatabasePool *pool): IDatabaseItem(pool) {
        m_attrType     = IDatabaseAttribute::Integer;
        m_nullsAllowed = true;
        m_unique       = false;
        m_candidateKey = false;
        m_refClass     = nullptr;
        m_refUnit      = nullptr;
        m_refLov       = nullptr;
        m_refNumerator = nullptr;
        m_parent       = nullptr;
    }

private:
    IDatabaseExpression createVariantExpr(
            QVariant value, IDatabaseExpression::ExpressionOperator oper)
    {
        IDatabaseExpression expr;
        IDatabaseAttribute *firstValue = dynamic_cast<IDatabaseAttribute *>(this);
        expr.setFirstValue(QVariant::fromValue(firstValue));
        expr.setSecondValue(value);
        expr.setExpressionOperator(oper);
        return expr;
    }

    IDatabaseExpression createAttrExpr(
            IDatabaseAttribute &value, IDatabaseExpression::ExpressionOperator oper)
    {
        IDatabaseExpression expr;
        IDatabaseAttribute *firstValue = dynamic_cast<IDatabaseAttribute *>(this);
        expr.setFirstValue(QVariant::fromValue(firstValue));
        expr.setSecondValue(QVariant::fromValue(&value));
        expr.setExpressionOperator(oper);
        return expr;
    }

    IDatabaseExpression createCompExpr(
            IDatabaseComposition &value, IDatabaseExpression::ExpressionOperator oper)
    {
        IDatabaseExpression expr;
        IDatabaseAttribute *firstValue = dynamic_cast<IDatabaseAttribute *>(this);
        expr.setFirstValue(QVariant::fromValue(firstValue));
        expr.setSecondValue(QVariant::fromValue(&value));
        expr.setExpressionOperator(oper);
        return expr;
    }

    //! Группа атрибута
    QString m_group;

    //! Значение по умолчанию
    QVariant m_initialValue;

    //! Нижняя граница атрибута
    QVariant m_lowerBound;

    //! Верхняя граница
    QVariant m_upperBound;

    //! Ссылочный класс
    IDatabaseClass *m_refClass;

    //! Единица измерения атрибута
    IDatabaseClass *m_refUnit;

    //! Список значений атрибута
    IDatabaseClass *m_refLov;

    //! Нумератор атрибута
    IDatabaseClass *m_refNumerator;

    //! Класс атрибута
    IDatabaseClass *m_parent;

    //! Тип атрибута
    AttrbuteType m_attrType;

    //! Максимальная длина
    qint32 m_length;

    //! Точность атрибута
    qint32 m_accuracy;

    //! Разрешение использовать пустое значение
    bool m_nullsAllowed;

    //! Уникальность значения атрибута
    bool m_unique;

    //! Является ли атрибут ключом
    bool m_candidateKey;
};

Q_DECLARE_METATYPE(IDatabaseAttribute*)

#endif
