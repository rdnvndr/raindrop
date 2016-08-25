#ifndef DBXMLSTRUCT_H
#define DBXMLSTRUCT_H

#include <QString>
#include <QStringList>

namespace  RTPTechGroup {
namespace  MetaDataModel {

//! Ключевые слова XML файла для описания Root класса
namespace DBROOTXML{
    const QString ROOT = "#document";
}

//! Ключевые слова XML файла для описания класса
namespace DBCLASSXML{
    const QString CLASS = "RTPClass";
    const QString NAME = "name";
    const QString MODE = "type";
    const QString TYPE = "view";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString TEMPLATE = "displayTemplate";
    const QString VERCOUNT = "versionCount";
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
    const QString ACCURACY = "accuracy";
    const QString ISNULLALLOWED = "isNullsAllowed";
    const QString ISUNIQUE = "isUnique";
    const QString ISCANDIDATEKEY = "isCandidateKey";
    const QString INITIALVAL = "initialValue";
    const QString LOWERBOUND = "lowerBound";
    const QString UPPERBOUND = "upperBound";
    const QString REFCLASS = "referencedClass";
    const QString REFUNIT =  "referencedUnit";
    const QString REFLOV =  "referencedLov";
    const QString REFNUMERATOR = "referenceNumerator";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания состава
namespace DBCOMPXML{
    const QString COMP = "RTPComposition";
    const QString LINKCLASS = "linkClass";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString CLASS = "Class";
    const QString ISVIEW = "isView";
    const QString DIRECTDESCRIPTION = "directDescription";
    const QString INVERSEDESCRIPTION = "inverseDescription";
    const QString ISCOMP = "isComp";
    const QString ID = "guid";
    const QString NAME = "name"; // Для удаления
}

//! Типы атрибутов
namespace DBATTRTYPEXML{
    const QString BOOLEAN = "Boolean";
    const QString BINARY = "Binary";
    const QString CHAR = "Char";
    const QString DATE = "Date";
    const QString DECIMAL = "Decimal";
    const QString DOUBLE = "Double";
    const QString INTEGER = "Integer";
    const QString STRING = "String";
    const QString REFERENCE = "Reference";
    const QString TIME = "Time";
    const QString TIMESHTAMP = "Timeshtamp";
}

//! Типы классов
namespace DBACCESSMODEXML{
    const QString SYSTEM = "System";
    const QString STANDART = "Standart";
    const QString USER = "User";
}

//!  Вид классов
namespace DBCLASSTYPEXML{
    const QString ABSTRACT = "Abstract";
    const QString CONTEXT  = "Context";
    const QString EMBEDDED = "Embedded";
    const QString NORMAL   = "Normal";
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

//! Ключевые слова XML файла для описания величины
namespace DBQUANTITYXML {
    const QString QUANTITY = "RTPMeasuredEntity";
    const QString BASICUNIT = "basicUnit";
    const QString ALIAS = "decription";
    const QString ID = "guid";
    const QString NAME = "name";
    const QString DIMENSION = "dimensionSymbol";
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

//! Ключевые слова XML файла для описания группы величины
namespace DBQUANTITYGROUPXML {
    const QString QUANTITYGROUP = "RTPEntityGroup";
    const QString NAME = "name";
    const QString ALIAS = "description";
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
namespace DBQUANTITYLISTXML {
    const QString QUANTITYLIST = "RTPEntityList";
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
    const QString REFLIST = "RTPRefList";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания групп справочников
namespace DBREFGROUPXML {
    const QString REFGROUP = "RTPRefGroup";
    const QString NAME = "name";
    const QString ALIAS = "description";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания справочника
namespace DBREFXML {
    const QString REF = "RTPRef";
    const QString NAME = "name";
    const QString ALIAS = "description";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания ссылок на класс
namespace DBLINKTOCLASSXML{
    const QString LINKTOCLASS = "RTPLinkToClass";
    const QString ALIAS = "alias";
    const QString REFCLASS = "refClass";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания ссылок на фильтр
namespace DBLINKTOFILTERXML{
    const QString LINKTOFILTER = "RTPLinkToFilter";
    const QString ALIAS = "alias";
    const QString REFFILTER = "refFilter";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания ссылок на справочник
namespace DBLINKTOREFXML{
    const QString LINKTOREF = "RTPLinkToRef";
    const QString ALIAS = "alias";
    const QString REFREF = "refRef";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для описания списка ролей
namespace DBROLELISTXML {
    const QString ROLELIST = "RTPRoleList";
    const QString NAME     = "name";
    const QString ALIAS    = "alias";
    const QString PARENT   = "parent";
    const QString ID       = "guid";
}

//! Ключевые слова XML файла для роли
namespace DBROLEXML{
    const QString ROLE        = "RTPRole";
    const QString NAME        = "name";
    const QString ALIAS       = "alias";
    const QString PARENT      = "parent";
    const QString ID          = "guid";
    const QString DESCRIPTION = "description";
}

//! Ключевые слова XML файла для прав доступа
namespace DBPERMISSIONXML{
    const QString PERMISSION = "RTPPermission";
    const QString ROLE       = "role";
    const QString ISCREATE   = "isCreate";
    const QString ISREAD     = "isRead";
    const QString ISWRITE    = "isWrite";
    const QString ISDELETE   = "isDelete";
    const QString ISBLOCK    = "isBlock";
    const QString PARENT     = "parent";
    const QString ID         = "guid";
}

//! Ключевые слова XML файла для описания списка нумераторов
namespace DBNUMERATORLISTXML {
    const QString NUMERATORLIST = "RTPNumeratorList";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для нумератора
namespace DBNUMERATORXML{
    const QString NUMERATOR = "RTPNumerator";
    const QString NAME = "name";
    const QString ALIAS = "alias";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для списка нумератора
namespace DBNUMERATORLOVXML{
    const QString NUMERATORLOV = "RTPNumeratorLOV";
    const QString REFLOV = "referencedLov";
    const QString PARENT = "parent";
    const QString ID = "guid";
}

//! Ключевые слова XML файла для регулярного выражения нумератора
namespace DBNUMERATORREGEXXML{
    const QString NUMERATORREGEX = "RTPNumeratorRegEx";
    const QString REGEX = "regex";
    const QString PARENT = "parent";
    const QString ID = "guid";
}


}}

#endif // DBXMLSTRUCT_H
