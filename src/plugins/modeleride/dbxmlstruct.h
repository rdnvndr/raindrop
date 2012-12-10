#ifndef DBXMLSTRUCT_H
#define DBXMLSTRUCT_H

#include <QString>
#include <QStringList>

//! Ключевые слова XML файла для описания класса
namespace DBCLASSXML{
    const QString CLASS = "RTPClass";
    const QString NAME = "name";
    const QString TYPE = "type";
    const QString ISABSTARCT = "isAbstract";
    const QString ISACTIVE = "isActive";
    const QString DESCRIPTION = "description";
    const QString PARENT = "parent";
    const QString TEMPLATE = "displayTemplate";
    const QString ID = "id";
}

//! Ключевые слова XML файла для описания класса
namespace DBATTRXML{
    const QString ATTR = "RTPAttribute";
    const QString NAME = "name";
    const QString DESCRIPTION = "description";
    const QString TYPE = "type";
    const QString GROUP = "group";
    const QString MAXSTRLEN = "maxStringLength";
    const QString ISNULLALLOWED = "isNullsAllowed";
    const QString ISUNIQUE = "isUnique";
    const QString ISCANDIDATEKEY = "isCandidateKey";
    const QString INITIALVAL = "initialValue";
    const QString REFCLASS = "referencedClass";
    const QString PARENT = "parent";
    const QString ID = "id";
}

//! Ключевые слова XML файла для описания состава
namespace DBCOMPXML{
    const QString COMP = "RTPComposition";
    const QString NAME = "name";
    const QString DESCRIPTION = "description";
    const QString PARENT = "parent";
    const QString CLASS = "Class";
    const QString DIRECTDESCRIPTION = "directDescription";
    const QString INVERSEDESCRIPTION = "inverseDescription";
    const QString ID = "id";
}

//! Список типов атрибута
const QStringList DBXMLATTRTYPE = (QStringList()
                               << "Boolean"
                               << "Binary"
                               << "Char"
                               << "Date"
                               << "Decimal"
                               << "Dimension"
                               << "Double"
                               << "Integer"
                               << "String"
                               << "Reference"
                               << "Range"
                               << "Time"
                               << "Timeshtamp"
                               );
//! Список типов класса
const QStringList DBXMLCLASSTYPE = (QStringList()
                               << "System"
                               << "Standart"
                               << "User"
                               );

#endif // DBXMLSTRUCT_H
