#ifndef IDATABASECLASS_H
#define IDATABASECLASS_H

#include <idatabaseitem.h>
#include <idatabaseattr.h>
#include <idatabasefilter.h>
#include <idatabaseexpression.h>

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
//    Q_ENUM(AccessMode)

    //! Перечисление типов класса
    enum ClassType { Abstract, Context, Embedded, Normal };
//    Q_ENUM(ClassType)

    //! Возращает иконку класса
    virtual QIcon icon() = 0;

    //! Устанавливает иконку класса
    virtual void setIcon(QIcon icon) = 0;

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
//    IDatabaseAttrs attrs;

     //! Получение отфильтрованных объектов класса
    virtual void filter(IDatabaseFilter *filter) = 0;

    //! Получение отфильтрованных объектов класса
    virtual void filter(IDatabaseExpression *expr) = 0;

    //! Получение всех объектов класса
    virtual void all() = 0;
};

//Q_DECLARE_INTERFACE(IDatabaseClass,"com.RTPTechGroup.Raindrop.IDatabaseClass/1.0")

#endif
