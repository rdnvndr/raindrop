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
    const QString GROUP = "type";
    const QString MAXSTRLEN = "maxStringLength";
    const QString ISNULLALLOWED = "isNullAllowed";
    const QString ISUNIQUE = "isUnique";
    const QString ISCANDIDATEKEY = "isCandidateKey";
    const QString INITIALVAL = "initialValue";
    const QString LOWVAL = "lowerBoundValue";
    const QString UPVAL = "upperBoundValue";
    const QString REFCLASS = "referencedClass";
    const QString PARENT = "parent";
}

const QStringList DBXMLTYPE = (QStringList()
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


#endif // DBXMLSTRUCT_H
