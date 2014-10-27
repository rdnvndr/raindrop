#ifndef DBXMLSTRUCT_H
#define DBXMLSTRUCT_H

#include <QString>
#include <QStringList>

//! Ключевые слова XML файла для описания Root класса
namespace DBROOTXML{
    const QString ROOT = "#document";
}

//! Ключевые слова XML файла для описания класса
namespace DBCLASSXML{
    const QString CLASS = "RTPClass";
    const QString NAME = "name";
    const QString TYPE = "type";
    const QString ISABSTARCT = "isAbstract";
    const QString ISACTIVE = "isActive";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString TEMPLATE = "displayTemplate";
    const QString ICON = "icon";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания класса
namespace DBATTRXML{
    const QString ATTR = "RTPAttribute";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString TYPE = "type";
    const QString GROUP = "group";
    const QString MAXSTRLEN = "maxStringLength";
    const QString ISNULLALLOWED = "isNullsAllowed";
    const QString ISUNIQUE = "isUnique";
    const QString ISCANDIDATEKEY = "isCandidateKey";
    const QString INITIALVAL = "initialValue";
    const QString LOWERBOUND = "lowerBound";
    const QString UPPERBOUND = "upperBound";
    const QString REFCLASS = "referencedClass";
    const QString REFUNIT =  "referencedUnit";
    const QString REFLOV =  "referencedLov";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания состава
namespace DBCOMPXML{
    const QString COMP = "RTPComposition";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString CLASS = "Class";
    const QString ISVIEW = "isView";
    const QString DIRECTDESCRIPTION = "directDescription";
    const QString INVERSEDESCRIPTION = "inverseDescription";
    const QString ISCOMP = "isComp";
    const QString ID = "guid";
}

//! Типы атрибутов
namespace DBTYPEXML{
    const QString BOOLEAN = "Boolean";
    const QString BINARY = "Binary";
    const QString CHAR = "Char";
    const QString DATE = "Date";
    const QString DECIMAL = "Decimal";
    const QString DIMENSION = "Dimension";
    const QString DOUBLE = "Double";
    const QString INTEGER = "Integer";
    const QString STRING = "String";
    const QString REFERENCE = "Reference";
    const QString RANGE = "Range";
    const QString TIME = "Time";
    const QString TIMESHTAMP = "Timeshtamp";
}

//! Типы классов
namespace DBCLASSTYPEXML{
    const QString SYSTEM = "System";
    const QString STANDART = "Standart";
    const QString USER = "User";
}

//! Ключевые слова XML файла для описания фильтра
namespace DBFILTERXML{
    const QString FILTER = "RTPFilter";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString CLASS = "Class";
    const QString DIRECTDESCRIPTION = "directDescription";
    const QString INVERSEDESCRIPTION = "inverseDescription";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания условий фильтра
namespace DBCONDITIONXML{
    const QString COND = "RTPCondition";
    const QString FIRSTATTR = "firstAttr";
    const QString PARENT = "parent";
    const QString SECONDATTR = "secondAttr";
    const QString OPERATOR = "operator";
    const QString LINKOF = "linkOf";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания условий фильтра
namespace DBFILTERBLOCKXML{
    const QString BLOCK = "RTPFilterBlock";
    const QString PARENT = "parent";
    const QString LINKOF = "linkOf";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания измеряемой сущности
namespace DBENTITYXML {
    const QString ENTITY = "RTPMeasuredEntity";
    const QString BASICUNIT = "basicUnit";
    const QString DESCRIPTION = "decription";
    const QString ID = "guid";
    const QString NAME = "name";
    const QString DIMENSIONSYMBOL = "dimensionSymbol";
    const QString PARENT = "parent";
}

//! Ключевые слова XML файла для описания единиц измерения
namespace DBUNITXML {
    const QString UNIT = "RTPMeasureUnit";
    const QString CODE = "code";
    const QString COEFF = "coeff";
    const QString DELTA = "delta";
    const QString NAME = "name";
    const QString ID = "guid";
    const QString DESIGNATION = "designation";
    const QString INTDESIGNATION = "intDesignation";
    const QString SYMBOL = "symbol";
    const QString INTSYMBOL = "intSymbol";
    const QString PARENT = "parent";
}

//! Ключевые слова XML файла для описания групп единиц измерения
namespace DBENTITYGROUPXML {
    const QString ENTITYGROUP = "RTPEntityGroup";
    const QString NAME = "name";
    const QString DESCRIPTION = "description";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания списка классов
namespace DBCLASSLISTXML {
    const QString CLASSLIST = "RTPClassList";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания списка единиц измерения
namespace DBENTITYLISTXML {
    const QString ENTITYLIST = "RTPEntityList";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания модели
namespace DBMODELXML {
    const QString MODEL = "RTPModel";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания списка списков значений
namespace DBLOVLISTXML {
    const QString LOVLIST = "RTPLOVList";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для списка значений
namespace DBLOVXML{
    const QString LOV = "RTPLOV";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString ID = "guid";
    const QString TYPE = "type";
}

//! Ключевые слова XML файла для значений списка
namespace DBLOVVALUEXML{
    const QString LOVVALUE = "RTPLOVValue";
    const QString NAME = "name";
    const QString VALUE = "value";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания списка справочников
namespace DBREFLISTXML {
    const QString CLASSLIST = "RTPRefList";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания групп справочников
namespace DBREFGROUPXML {
    const QString REFGROUP = "RTPRefGroup";
    const QString NAME = "name";
    const QString DESCRIPTION = "description";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания ссылок на класс
namespace DBLINKTOCLASSXML{
    const QString LINKTOCLASS = "RTPLinkToClass";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString REFCLASS = "refClass";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания ссылок на фильтр
namespace DBLINKTOFILTERXML{
    const QString LINKTOFILTER = "RTPLinkToFilter";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString REFFILTER = "refFilter";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания справочника
namespace DBREFXML {
    const QString REF = "RTPRef";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

#endif // DBXMLSTRUCT_H
