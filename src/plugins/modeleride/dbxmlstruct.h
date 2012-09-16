#ifndef DBXMLSTRUCT_H
#define DBXMLSTRUCT_H
#include <QString>
#include <QStringList>
namespace DBCLASSXML{
    const QString CLASS = "RTPClass";
    const QString CLASSNAME = "className";
    const QString ISSYSTEM = "isSystem";
    const QString ISABSTARCT = "isAbstract";
    const QString DESCRIPTION = "description";
    const QString PARENT = "parent";
}

namespace DBATTRXML{
    const QString ATTR = "RTPAttribute";
    const QString ATTRNAME = "attributeName";
    const QString DESCRIPTION = "description";
    const QString ATTRTYPE = "attributeType";
    const QString MAXSTRLEN = "maxStringLength";
    const QString ISARRAY = "isArray";
    const QString ARRAYLEN = "arrayLength";
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
                               << "String"
                               << "Referenced"
                               << "Double"
                               << "Int"
                               );


#endif // DBXMLSTRUCT_H
