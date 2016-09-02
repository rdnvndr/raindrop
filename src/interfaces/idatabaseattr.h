#ifndef IDATABASEATTR_H
#define IDATABASEATTR_H

#include <QHash>
#include <QVariant>
#include <QUuid>

#include <idatabaseexpression.h>
#include <idatabaseitem.h>
#include <idatabaseclass.h>

//class IDatabaseExpression;
class IDatabaseClass;
class IDatabaseAttr;

typedef QHash<QString, IDatabaseAttr *> IDatabaseAttrs;

//! Атрибут класса базы данных
class IDatabaseAttr: public IDatabaseItem
{
public:
    //! Перечисление типов атрибутов
    enum AttrType { Boolean, Binary, Char, Date, Decimal, Double, Integer,
                    String, Reference, Time, Timeshtamp };
//    Q_ENUM(AttrType)

    //! Возращает тип атрибута
    virtual AttrType attrType() = 0;

    //! Устанавливает тип атрибута
    virtual void setAttrType(AttrType attrType) = 0;

     //! Возращает группу атрибута
    virtual QString group() = 0;

    //! Устанавливает группу атрибута
    virtual void setGroup(const QString &group) = 0;

     //! Возращает максимальную длину
    virtual qint32 maxStringLength() = 0;

    //! Устанавливает максимальную длину
    virtual void setMaxStringLength(qint32 length) = 0;

     //! Возращает точность
    virtual qint32 accuracy() = 0;

    //! Устанавливает точность
    virtual void setAccuracy(qint32 accuracy) = 0;

     //! Возращает разрешение присваивать NULL
    virtual bool isNullsAllowed() = 0;

    //! Устанавливает разрешение присваивать NULL
    virtual void setNullsAllowed(bool nullsAllowed) = 0;

     //! Возращает уникальность атрибута
    virtual bool isUnique() = 0;

    //! Устанавливает уникальность атрибута
    virtual void setUnique(bool unique) = 0;

     //! Возращает является ли атрибут ключом
    virtual bool isCandidateKey() = 0;

    //! Устанавливает атрибут ключом
    virtual void setCandidateKey(bool candidateKey) = 0;

     //! Возращает значение по умолчанию
    virtual QVariant initialValue() = 0;

    //! Устанавливает значение по умолчанию
    virtual void setInitialValue(QVariant initialValue) = 0;

     //! Возращает нижнюю границу атрибута
    virtual QVariant lowerBound() = 0;

    //! Устанавливает нижнюю границу атрибута
    virtual void setLowerBound(QVariant lowerBound) = 0;

     //! Возращает верхнюю границу атрибута
    virtual QVariant upperBound() = 0;

    //! Устанавливает верхнюю границу атрибута
    virtual void setUpperBound(QVariant upperBound) = 0;

     //! Возращает ссылочный класс
    virtual IDatabaseClass *referencedClass() = 0;

    //! Устанавливает ссылочный класс
    virtual void setReferencedClass(IDatabaseClass *refClass) = 0;

     //! Возращает единицу измерения атрибута
    virtual IDatabaseClass *referencedUnit() = 0;

    //! Устанавливает единицу измерения атрибута
    virtual void setReferencedUnit(IDatabaseClass *refUnit) = 0;

     //! Возращает список значений атрибута
    virtual IDatabaseClass *referencedLov() = 0;

    //! Устанавливает список значений атрибута
    virtual void setReferencedLov(IDatabaseClass *refLov) = 0;

     //! Возращает нумератор для атрибута
    virtual IDatabaseClass *referenceNumerator() = 0;

    //! Устанавливает нумератор для атрибута
    virtual void setReferenceNumerator(IDatabaseClass *refNumerator) = 0;

     //! Возращает класс атрибута
    virtual IDatabaseClass *parent() = 0;

    //! Устанавливает класс атрибута
    virtual void setParent(IDatabaseClass *parent) = 0;

    //! Проверка наследования атрибута
    virtual bool isInherited(IDatabaseClass *parent) = 0;

// Работа с выражениями
    //! Формирует выражение равенства
    IDatabaseExpression operator == (QVariant value)
    {
        return createVariantExpression(value, IDatabaseExpression::EQ);
    }

    //! Формирует выражение равенства
    IDatabaseExpression operator == (IDatabaseAttr &value)
    {
        return createAttrExpression(value, IDatabaseExpression::EQ);
    }

    //! Формирует выражение неравенства
    IDatabaseExpression operator != (QVariant value)
    {
        return createVariantExpression(value, IDatabaseExpression::NE);
    }

    //! Формирует выражение неравенства
    IDatabaseExpression operator != (IDatabaseAttr &value)
    {
        return createAttrExpression(value, IDatabaseExpression::NE);
    }

    //! Формирует выражение больше или равно
    IDatabaseExpression operator >= (QVariant value)
    {
        return createVariantExpression(value, IDatabaseExpression::GE);
    }

    //! Формирует выражение больше или равно
    IDatabaseExpression operator >= (IDatabaseAttr &value)
    {
        return createAttrExpression(value, IDatabaseExpression::GE);
    }

    //! Формирует выражение меньше или равно
    IDatabaseExpression operator <= (QVariant value)
    {
        return createVariantExpression(value, IDatabaseExpression::LE);
    }

    //! Формирует выражение меньше или равно
    IDatabaseExpression operator <= (IDatabaseAttr &value)
    {
        return createAttrExpression(value, IDatabaseExpression::LE);
    }

    //! Формирует выражение больше
    IDatabaseExpression operator >  (QVariant value)
    {
        return createVariantExpression(value, IDatabaseExpression::GT);
    }

    //! Формирует выражение больше
    IDatabaseExpression operator >  (IDatabaseAttr &value)
    {
        return createAttrExpression(value, IDatabaseExpression::GT);
    }

    //! Формирует выражение меньше
    IDatabaseExpression operator <  (QVariant value)
    {
        return createVariantExpression(value, IDatabaseExpression::LT);
    }

    //! Формирует выражение меньше
    IDatabaseExpression operator <  (IDatabaseAttr &value)
    {
        return createAttrExpression(value, IDatabaseExpression::LT);
    }

    IDatabaseExpression createVariantExpression(
            QVariant value, IDatabaseExpression::ExpressionOperator oper)
    {
        IDatabaseExpression expr;
        IDatabaseAttr *firstValue = dynamic_cast<IDatabaseAttr *>(this);
        expr.setFirstValue(QVariant::fromValue(firstValue));
        expr.setSecondValue(value);
        expr.setExpressionOperator(oper);
        return expr;
    }

    IDatabaseExpression createAttrExpression(
            IDatabaseAttr &value, IDatabaseExpression::ExpressionOperator oper)
    {
        IDatabaseExpression expr;
        IDatabaseAttr *firstValue = dynamic_cast<IDatabaseAttr *>(this);
        expr.setFirstValue(QVariant::fromValue(firstValue));
        expr.setSecondValue(QVariant::fromValue(&value));
        expr.setExpressionOperator(oper);
        return expr;
    }

};

//Q_DECLARE_OPAQUE_POINTER(IDatabaseAttr*)
Q_DECLARE_METATYPE(IDatabaseAttr*)

//Q_DECLARE_INTERFACE(IDatabaseAttr,"com.RTPTechGroup.Raindrop.IDatabaseAttr/1.0")

#endif
