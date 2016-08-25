#ifndef PGDATABASECLASS_H
#define PGDATABASECLASS_H

#include <idatabaseclass.h>

class PgDatabaseClass : public IDatabaseClass
{
public:
    PgDatabaseClass();

    //! Возращает идентификатор класса
    QUuid id();

    //! Устанавливает идентификатор класса
    void setId(QUuid id);

    //! Возращает иконку класса
    QIcon icon();

    //! Устанавливает иконку класса
    void setIcon(QIcon icon);

    //! Возращает имя класса
    QString name();

    //! Устанавлвает имя класса
    void setName(const QString &name);

    //! Возращает псевдоним класса
    QString alias();

    //! Устанавливает псевдоним класса
    void setAlias(const QString &alias);

    //! Возращает тип класса
    ClassType classType();

    //! Устанавливает тип класса
    void setClassType(ClassType classType);

    //! Возращает режим доступа
    AccessMode accessMode();

    //! Устанавливает режим доступа
    void setAccessMode(AccessMode accessMode);

    //!  Возращает родительский класс
    IDatabaseClass *parent();

    //!  Устанавливает родительский класс
    void setParent(IDatabaseClass *parent);

    //!  Возращает шаблон имени объекта
    QString objectNameTemplate();

    //!  Устанавливает шаблон имени объекта
    void setObjectNameTemplate(const QString &objectNameTemplate);

    //! Возращает максимальное количество версий объекта
    qint32 maxVersion();

    //! Устанавливает максимальное количество версий объекта
    void setMaxVersion(qint32 maxVersion);

    //! Cписок атрибутов в классе
    IDatabaseAttrList attrList();
};

#endif // PGDATABASECLASS_H
