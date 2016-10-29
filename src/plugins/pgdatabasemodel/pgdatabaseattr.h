#ifndef PGDATABASEATTR_H
#define PGDATABASEATTR_H

#include <idatabaseattr.h>

//! Реализация атрибута базы данных
class PgDatabaseAttr : public IDatabaseAttr
{
public:
    //! Конструктор класса
    explicit PgDatabaseAttr();

    //! Деструктор класса
    virtual ~PgDatabaseAttr();

    //! Возращает идентификатор элемента базы данных
    QUuid id();

    //! Устанавливает идентификатор элемента базы данных
    void setId(QUuid id);

    //! Возращает имя элемента базы данных
    QString name();

    //! Устанавливает имя элемента базы данных
    void setName(const QString &name);

    //! Возращает псевдоним элемента базы данных
    QString alias();

    //! Устанавливает псевдоним элемента базы данных
    void setAlias(const QString &alias);

    //! Проверка существования элемента базы данных
    bool isExist();

    //! Создание элемента базы данных
    bool push();

    //! Обновление элемента базы данных
    bool pull();

    //! Удаление элемента базы данных
    bool remove();

    //! Возращает тип атрибута
    AttrType attrType();

    //! Устанавливает тип атрибута
    void setAttrType(AttrType attrType);

     //! Возращает группу атрибута
    QString group();

    //! Устанавливает группу атрибута
    void setGroup(const QString &group);

     //! Возращает максимальную длину
    qint32 maxStringLength();

    //! Устанавливает максимальную длину
    void setMaxStringLength(qint32 length);

     //! Возращает точность
    qint32 accuracy();

    //! Устанавливает точность
    void setAccuracy(qint32 accuracy);

     //! Возращает разрешение присваивать NULL
    bool isNullsAllowed();

    //! Устанавливает разрешение присваивать NULL
    void setNullsAllowed(bool nullsAllowed);

     //! Возращает уникальность атрибута
    bool isUnique();

    //! Устанавливает уникальность атрибута
    void setUnique(bool unique);

     //! Возращает является ли атрибут ключом
    bool isCandidateKey();

    //! Устанавливает атрибут ключом
    void setCandidateKey(bool candidateKey);

     //! Возращает значение по умолчанию
    QVariant initialValue();

    //! Устанавливает значение по умолчанию
    void setInitialValue(QVariant initialValue);

     //! Возращает нижнюю границу атрибута
    QVariant lowerBound();

    //! Устанавливает нижнюю границу атрибута
    void setLowerBound(QVariant lowerBound);

     //! Возращает верхнюю границу атрибута
    QVariant upperBound();

    //! Устанавливает верхнюю границу атрибута
    void setUpperBound(QVariant upperBound);

     //! Возращает ссылочный класс
    IDatabaseClass *referencedClass();

    //! Устанавливает ссылочный класс
    void setReferencedClass(IDatabaseClass *refClass);

     //! Возращает единицу измерения атрибута
    IDatabaseClass *referencedUnit();

    //! Устанавливает единицу измерения атрибута
    void setReferencedUnit(IDatabaseClass *refUnit);

     //! Возращает список значений атрибута
    IDatabaseClass *referencedLov();

    //! Устанавливает список значений атрибута
    void setReferencedLov(IDatabaseClass *refLov);

     //! Возращает нумератор для атрибута
    IDatabaseClass *referenceNumerator();

    //! Устанавливает нумератор для атрибута
    void setReferenceNumerator(IDatabaseClass *refNumerator);

     //! Возращает класс атрибута
    IDatabaseClass *parent();

    //! Устанавливает класс атрибута
    void setParent(IDatabaseClass *parent);

    //! Проверка наследования атрибута
    bool isInherited(IDatabaseClass *parent);

// Работа с выражениями
    //! Формирует выражение равенства
    IDatabaseExpression &operator == (QVariant value);

    //! Формирует выражение равенства
    IDatabaseExpression &operator == (IDatabaseAttr &value);

    //! Формирует выражение неравенства
    IDatabaseExpression& operator != (QVariant value);

    //! Формирует выражение неравенства
    IDatabaseExpression& operator != (IDatabaseAttr &value);

    //! Формирует выражение больше или равно
    IDatabaseExpression& operator >= (QVariant value);

    //! Формирует выражение больше или равно
    IDatabaseExpression& operator >= (IDatabaseAttr &value);

    //! Формирует выражение меньше или равно
    IDatabaseExpression& operator <= (QVariant value);

    //! Формирует выражение меньше или равно
    IDatabaseExpression& operator <= (IDatabaseAttr &value);

    //! Формирует выражение больше
    IDatabaseExpression& operator >  (QVariant value);

    //! Формирует выражение больше
    IDatabaseExpression& operator >  (IDatabaseAttr &value);

    //! Формирует выражение меньше
    IDatabaseExpression& operator <  (QVariant value);

    //! Формирует выражение меньше
    IDatabaseExpression& operator <  (IDatabaseAttr &value);

private:
    //! Формирование выражения для указанного оператора
    IDatabaseExpression& createVariantExpression(
            QVariant value, IDatabaseExpression::ExpressionOperator oper);

    //! Формирование выражения для указанного оператора
    IDatabaseExpression& createAttrExpression(
            IDatabaseAttr &value, IDatabaseExpression::ExpressionOperator oper);
};

#endif // PGDATABASEATTR_H
