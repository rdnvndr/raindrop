#ifndef DBXMLSTRUCT_H
#define DBXMLSTRUCT_H
#include <QString>
#include <QStringList>
namespace DBCLASSXML{
    const QString CLASS = "RTPClass";
    const QString NAME = "name";
    const QString TYPE = "type";
    const QString ISABSTARCT = "isAbstract";
    const QString ISACTIVE = "isActive";
    const QString DESCRIPTION = "description";
    const QString PARENT = "parent";
    const QString TEMPLATE = "displayTemplate";
}

namespace DBATTRXML{
    const QString ATTR = "RTPAttribute";
    const QString NAME = "name";
    const QString DESCRIPTION = "description";
    const QString TYPE = "type";
    const QString GROUP = "group";
    const QString MAXSTRLEN = "maxStringLength";
    const QString ISNULLALLOWED = "isNullAllowed";
    const QString ISUNIQUE = "isUnique";
    const QString ISCANDIDATEKEY = "isCandidateKey";
    const QString INITIALVAL = "initialValue";
    const QString REFCLASS = "referencedClass";
    const QString PARENT = "parent";
}

const QStringList DBXMLATTRTYPE = (QStringList()
                               << "Boolean"
                               << "Char"
                               << "Date"
                               << "Decimal"
                               << "Double"
                               << "Integer"
                               << "String"
                               << "Reference"
                               << "Time"
                               << "Timeshtamp"
                               );

const QStringList DBXMLCLASSTYPE = (QStringList()
                               << "System"
                               << "Standart"
                               << "User"
                               );

#endif // DBXMLSTRUCT_H
