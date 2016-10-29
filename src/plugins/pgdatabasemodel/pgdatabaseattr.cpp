#include "pgdatabaseattr.h"

#include "pgdatabaseexpression.h"

#include <QDebug>
#include <QVariant>

PgDatabaseAttr::PgDatabaseAttr()
{

}

PgDatabaseAttr::~PgDatabaseAttr()
{

}

QUuid PgDatabaseAttr::id()
{
    return QUuid();
}

void PgDatabaseAttr::setId(QUuid id)
{

}

QString PgDatabaseAttr::name()
{
    return QString();
}

void PgDatabaseAttr::setName(const QString &name)
{

}

QString PgDatabaseAttr::alias()
{
    return QString();
}

void PgDatabaseAttr::setAlias(const QString &alias)
{

}

bool PgDatabaseAttr::isExist()
{
    return true;
}

bool PgDatabaseAttr::push()
{
    return true;
}

bool PgDatabaseAttr::pull()
{
    return true;
}

bool PgDatabaseAttr::remove()
{
    return true;
}


IDatabaseAttr::AttrType PgDatabaseAttr::attrType()
{
    return AttrType::Binary;
}

void PgDatabaseAttr::setAttrType(IDatabaseAttr::AttrType attrType)
{

}

QString PgDatabaseAttr::group()
{
    return QString();
}

void PgDatabaseAttr::setGroup(const QString &group)
{

}

qint32 PgDatabaseAttr::maxStringLength()
{
    return 0;
}

void PgDatabaseAttr::setMaxStringLength(qint32 length)
{

}

qint32 PgDatabaseAttr::accuracy()
{
    return 0;
}

void PgDatabaseAttr::setAccuracy(qint32 accuracy)
{

}

bool PgDatabaseAttr::isNullsAllowed()
{
    return true;
}

void PgDatabaseAttr::setNullsAllowed(bool nullsAllowed)
{

}

bool PgDatabaseAttr::isUnique()
{
    return true;
}

void PgDatabaseAttr::setUnique(bool unique)
{

}

bool PgDatabaseAttr::isCandidateKey()
{
    return true;
}

void PgDatabaseAttr::setCandidateKey(bool candidateKey)
{

}

QVariant PgDatabaseAttr::initialValue()
{
    return QVariant();
}

void PgDatabaseAttr::setInitialValue(QVariant initialValue)
{

}

QVariant PgDatabaseAttr::lowerBound()
{
    return QVariant();
}

void PgDatabaseAttr::setLowerBound(QVariant lowerBound)
{

}

QVariant PgDatabaseAttr::upperBound()
{
    return QVariant();
}

void PgDatabaseAttr::setUpperBound(QVariant upperBound)
{

}

IDatabaseClass *PgDatabaseAttr::referencedClass()
{
    return NULL;
}

void PgDatabaseAttr::setReferencedClass(IDatabaseClass *refClass)
{

}

IDatabaseClass *PgDatabaseAttr::referencedUnit()
{
    return NULL;
}

void PgDatabaseAttr::setReferencedUnit(IDatabaseClass *refUnit)
{

}

IDatabaseClass *PgDatabaseAttr::referencedLov()
{
    return NULL;
}

void PgDatabaseAttr::setReferencedLov(IDatabaseClass *refLov)
{

}

IDatabaseClass *PgDatabaseAttr::referenceNumerator()
{
    return NULL;
}

void PgDatabaseAttr::setReferenceNumerator(IDatabaseClass *refNumerator)
{

}

IDatabaseClass *PgDatabaseAttr::parent()
{
    return NULL;
}

void PgDatabaseAttr::setParent(IDatabaseClass *parent)
{

}

bool PgDatabaseAttr::isInherited(IDatabaseClass *parent)
{
    return true;
}


