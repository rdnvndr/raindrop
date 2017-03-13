#ifndef IDATABASECLASS_H
#define IDATABASECLASS_H

#include <idatabaseitem.h>
#include <idatabaseattribute.h>
#include <idatabasefilter.h>
#include <idatabasecomposition.h>
#include <idatabaseexpression.h>
#include <idatabaselist.h>

#include <QIcon>
#include <QUuid>
#include <QString>

class IDatabaseExpression;

//! Класс базы данных
class IDatabaseClass: public IDatabaseItem
{
public:
    //! Перечисление режимов доступа
    enum AccessMode { System, Standart, User };

    //! Перечисление типов класса
    enum ClassType { Abstract, Context, Embedded, Normal };

    //! Конструктор класса
    explicit IDatabaseClass() {
        m_classType  = IDatabaseClass::Normal;
        m_accessMode = IDatabaseClass::Standart;
        m_parent     = NULL;
    }

    //! Деструктор класса
    virtual ~IDatabaseClass() {}


    //! Возращает иконку класса
    virtual QIcon icon() { return m_icon; }

    //! Устанавливает иконку класса
    virtual void setIcon(QIcon icon) { m_icon = icon; }

    //! Возращает тип класса
    virtual ClassType classType() { return m_classType; }

    //! Устанавливает тип класса
    virtual void setClassType(ClassType classType) { m_classType = classType; }

    //! Возращает режим доступа
    virtual AccessMode accessMode() { return m_accessMode; }

    //! Устанавливает режим доступа
    virtual void setAccessMode(AccessMode accessMode)
    { m_accessMode = accessMode; }

    //!  Возращает родительский класс
    virtual IDatabaseClass *parent() { return m_parent; }

    //!  Устанавливает родительский класс
    virtual void setParent(IDatabaseClass *parent) { m_parent = parent; }

    //!  Возращает шаблон имени объекта
    virtual QString objectNameTemplate() { return m_objectNameTemplate; }

    //!  Устанавливает шаблон имени объекта
    virtual void setObjectNameTemplate(const QString &objectNameTemplate)
    { m_objectNameTemplate = objectNameTemplate; }

    //! Возращает максимальное количество версий объекта
    virtual qint32 maxVersion() { return m_maxVersion; }

    //! Устанавливает максимальное количество версий объекта
    virtual void setMaxVersion(qint32 maxVersion) { m_maxVersion = maxVersion; }

// Создание элемента класса по имени
    //! Создание производного класса по имени
    virtual IDatabaseClass *createDerivedClass(const QString &name) = 0;

    //! Создание атрибута по имени
    virtual IDatabaseAttribute *createAttr(const QString &name) = 0;

    //! Создание фильтра по имени
    virtual IDatabaseFilter *createFilter(const QString &name) = 0;

    //! Создание состава по имени
    virtual IDatabaseComposition *createComp(const QString &name) = 0;

// Получение доступа к элементам класса по имени
    //! Получение производного класса по имени
    virtual IDatabaseClass *derivedClass(const QString &name) = 0;

    //! Получение атрибута по имени
    virtual IDatabaseAttribute *attr(const QString &name) = 0;

    //! Получение фильтра по имени
    virtual IDatabaseFilter *filter(const QString &name) = 0;

    //! Получение состава по имени
    virtual IDatabaseComposition *comp(const QString &name) = 0;

// Получение доступа к спискам элементов класса по имени
    //! Cписок производных классов
    virtual IDatabaseClasses *classList() = 0;

    //! Cписок атрибутов в классе
    virtual IDatabaseAttrs *attrList() = 0;

    //! Cписок фильтров в классе
    virtual IDatabaseFilters *filterList() = 0;

    //! Cписок атрибутов в классе
    virtual IDatabaseComps *compList() = 0;

// Работа с объектами
     //! Получение отфильтрованных объектов класса
    virtual void filtering(IDatabaseFilter *filter) = 0;

    //! Получение отфильтрованных объектов класса
    virtual void filtering(IDatabaseExpression *expr) = 0;

    //! Получение всех объектов класса
    virtual void all() = 0;

private:
    //! Иконка класса
    QIcon m_icon;

    //! Тип класса
    ClassType m_classType;

    //! Режим доступа
    AccessMode m_accessMode;

    //! Родительский класс
    IDatabaseClass *m_parent;

    //! Шаблон имени объекта
    QString m_objectNameTemplate;

    //! Максимальное количество версий объекта
    qint32 m_maxVersion;

};

//Q_DECLARE_INTERFACE(IDatabaseClass,"com.RTPTechGroup.Raindrop.IDatabaseClass/1.0")

#endif
