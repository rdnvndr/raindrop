#ifndef IDATABASECLASS_H
#define IDATABASECLASS_H

#include <idatabaseitem.h>
#include <idatabaseattribute.h>
#include <idatabasefilter.h>
#include <idatabasecomposition.h>
#include <idatabaseobjects.h>

#include <QIcon>
#include <QUuid>
#include <QString>

//! Класс базы данных
class IDatabaseClass: public IDatabaseItem, public IDatabaseManipulation
{
public:
    //! Перечисление режимов доступа
    enum AccessMode { System,   //!< Системный
                      Standart, //!< Стандартный
                      User      //!< Пользовательский
                    };

    //! Перечисление типов класса
    enum ClassType { Abstract, //!< Абстрактный
                     Context,  //!< Контекст
                     Embedded, //!< Встроенный
                     Normal    //!< Обычный
                   };

    //! Конструктор класса
    explicit IDatabaseClass(): IDatabaseManipulation(this) {
        m_classType  = IDatabaseClass::Normal;
        m_accessMode = IDatabaseClass::Standart;
        m_baseClass  = nullptr;
    }

    //! Деструктор класса
    virtual ~IDatabaseClass() {}


    //! Возвращает иконку класса
    virtual QIcon icon() const { return m_icon; }

    //! Устанавливает иконку класса
    virtual void setIcon(const QIcon &icon) { m_icon = icon; }

    //! Возвращает тип класса
    virtual ClassType classType() { return m_classType; }

    //! Устанавливает тип класса
    virtual void setClassType(ClassType classType) { m_classType = classType; }

    //! Возвращает режим доступа
    virtual AccessMode accessMode() { return m_accessMode; }

    //! Устанавливает режим доступа
    virtual void setAccessMode(AccessMode accessMode)
    { m_accessMode = accessMode; }

    //!  Возвращает базовый класс
    virtual IDatabaseClass *baseClass() { return m_baseClass; }

    //!  Устанавливает базовый класс
    virtual void setBaseClass(IDatabaseClass *parent) { m_baseClass = parent; }

    //!  Возвращает шаблон имени объекта
    virtual QString objectNameTemplate() { return m_objectNameTemplate; }

    //!  Устанавливает шаблон имени объекта
    virtual void setObjectNameTemplate(const QString &objectNameTemplate)
    { m_objectNameTemplate = objectNameTemplate; }

    //! Возвращает максимальное количество версий объекта
    virtual qint32 maxVersion() { return m_maxVersion; }

    //! Устанавливает максимальное количество версий объекта
    virtual void setMaxVersion(qint32 maxVersion) { m_maxVersion = maxVersion; }

// Создание элемента класса по имени
    //! Создание производного класса по имени
    virtual IDatabaseClass *createClass(const QString &name) = 0;

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
    //! Список производных классов
    virtual IDatabaseClasses *derivedClassList() = 0;

    //! Список атрибутов в классе
    virtual IDatabaseAttrs *attrList() = 0;

    //! Список фильтров в классе
    virtual IDatabaseFilters *filterList() = 0;

    //! Список атрибутов в классе
    virtual IDatabaseComps *compList() = 0;

private:
    //! Иконка класса
    QIcon m_icon;

    //! Тип класса
    ClassType m_classType;

    //! Режим доступа
    AccessMode m_accessMode;

    //! Родительский класс
    IDatabaseClass *m_baseClass;

    //! Шаблон имени объекта
    QString m_objectNameTemplate;

    //! Максимальное количество версий объекта
    qint32 m_maxVersion;

};

//Q_DECLARE_INTERFACE(IDatabaseClass,"com.RTPTechGroup.Raindrop.IDatabaseClass/1.0")

#endif
