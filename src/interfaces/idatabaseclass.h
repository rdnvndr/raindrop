#ifndef IDATABASECLASS_H
#define IDATABASECLASS_H

#include <idatabaseattr.h>

#include <QIcon>
#include <QUuid>
#include <QString>

//! Класс базы данных
class IDatabaseClass
{
public:
    //! Перечисление режимов доступа
    enum AccessMode { System, Standart, User };
    Q_ENUM(AccessMode)

    //! Перечисление типов класса
    enum ClassType { Abstract, Context, Embedded, Normal };
    Q_ENUM(ClassType)

    //! Возращает идентификатор класса
    virtual QUuid id() = 0;

    //! Устанавливает идентификатор класса
    virtual void setId(QUuid id) = 0;

    //! Возращает иконку класса
    virtual QIcon icon() = 0;

    //! Устанавливает иконку класса
    virtual void setIcon(QIcon icon) = 0;

    //! Возращает имя класса
    virtual QString name() = 0;

    //! Устанавлвает имя класса
    virtual void setName(const QString &name) = 0;

    //! Возращает псевдоним класса
    virtual QString alias() = 0;

    //! Устанавливает псевдоним класса
    virtual void setAlias(const QString &alias) = 0;

    //! Возращает тип класса
    virtual ClassType classType() = 0;

    //! Устанавливает тип класса
    virtual void setClassType(ClassType classType) = 0;

    //! Возращает режим доступа
    virtual AccessMode accessMode() = 0;

    //! Устанавливает режим доступа
    virtual void setAccessMode(AccessMode accessMode) = 0;

    //!  Возращает родительский класс
    virtual IDatabaseClass *parent() = 0;

    //!  Устанавливает родительский класс
    virtual void setParent(IDatabaseClass *parent) = 0;

    //!  Возращает шаблон имени объекта
    virtual QString objectNameTemplate() = 0;

    //!  Устанавливает шаблон имени объекта
    virtual void setObjectNameTemplate(const QString &objectNameTemplate) = 0;

    //! Возращает максимальное количество версий объекта
    virtual qint32 maxVersion() = 0;

    //! Устанавливает максимальное количество версий объекта
    virtual void setMaxVersion(qint32 maxVersion) = 0;

    //! Cписок атрибутов в классе
    virtual IDatabaseAttrList attrList() = 0;

private:

}

Q_DECLARE_INTERFACE(IDatabaseClass,"com.RTPTechGroup.Raindrop.IDatabaseClass/1.0")
#endif
