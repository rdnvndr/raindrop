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
    const QString CLASS    = "RTPClass";
    const QString NAME     = "name";
    const QString MODE     = "accessMode";
    const QString TYPE     = "type";
    const QString ALIAS    = "alias";
    const QString PARENT   = "parent";
    const QString TEMPLATE = "displayTemplate";
    const QString VERCOUNT = "versionCount";
    const QString ICON     = "icon";
    const QString ID       = "guid";
}

//! Ключевые слова XML файла для описания класса
namespace DBATTRXML{
    const QString ATTR           = "RTPAttribute";
    const QString NAME           = "name";
    const QString ALIAS          = "alias";
    const QString TYPE           = "type";
    const QString GROUP          = "group";
    const QString MAXSTRLEN      = "maxStringLength";
    const QString ACCURACY       = "accuracy";
    const QString ISNULLALLOWED  = "isNullsAllowed";
    const QString ISUNIQUE       = "isUnique";
    const QString ISCANDIDATEKEY = "isCandidateKey";
    const QString INITIALVAL     = "initialValue";
    const QString LOWERBOUND     = "lowerBound";
    const QString UPPERBOUND     = "upperBound";
    const QString REFCLASS       = "referencedClass";
    const QString REFUNIT        = "referencedUnit";
    const QString REFLOV         = "referencedLov";
    const QString REFNUMERATOR   = "referenceNumerator";
    const QString PARENT         = "parent";
    const QString ID             = "guid";
}

//! Ключевые слова XML файла для описания состава
namespace DBCOMPXML{
    const QString COMP               = "RTPComposition";
    const QString LINKCLASS          = "linkClass";
    const QString ALIAS              = "alias";
    const QString PARENT             = "parent";
    const QString CLASS              = "Class";
    const QString ISVIEW             = "isView";
    const QString DIRECTDESCRIPTION  = "directDescription";
    const QString INVERSEDESCRIPTION = "inverseDescription";
    const QString ISCOMP             = "isComp";
    const QString ID                 = "guid";
    const QString NAME               = "name";               // Для удаления
}

//! Типы атрибутов
namespace DBATTRTYPEXML{
    const QString ATTRTYPE   = "RTPAttributeType";
    const QString BOOLEAN    = "Boolean";
    const QString BINARY     = "Binary";
    const QString CHAR       = "Char";
    const QString DATE       = "Date";
    const QString DECIMAL    = "Decimal";
    const QString DOUBLE     = "Double";
    const QString INTEGER    = "Integer";
    const QString STRING     = "String";
    const QString REFERENCE  = "Reference";
    const QString TIME       = "Time";
    const QString TIMESHTAMP = "Timeshtamp";
}

//! Типы классов
namespace DBACCESSMODEXML{
    const QString ACCESSMODE = "RTPAccessMode";
    const QString SYSTEM     = "System";
    const QString STANDART   = "Standart";
    const QString USER       = "User";
}

//!  Вид классов
namespace DBCLASSTYPEXML{
    const QString CLASSTYPE = "RTPClassType";
    const QString ABSTRACT  = "Abstract";
    const QString CONTEXT   = "Context";
    const QString EMBEDDED  = "Embedded";
    const QString NORMAL    = "Normal";
}

//! Ключевые слова XML файла для описания фильтра
namespace DBFILTERXML{
    const QString FILTER             = "RTPFilter";
    const QString NAME               = "name";
    const QString ALIAS              = "alias";
    const QString PARENT             = "parent";
    const QString CLASS              = "Class";
    const QString DIRECTDESCRIPTION  = "directDescription";
    const QString INVERSEDESCRIPTION = "inverseDescription";
    const QString ID                 = "guid";
}

//! Ключевые слова XML файла для описания условий фильтра
namespace DBCONDITIONXML{
    const QString COND       = "RTPCondition";
    const QString FIRSTATTR  = "firstAttr";
    const QString PARENT     = "parent";
    const QString SECONDATTR = "secondAttr";
    const QString OPERATOR   = "operator";
    const QString LINKOF     = "linkOf";
    const QString ID         = "guid";
}

//! Ключевые слова XML файла для описания условий фильтра
namespace DBFILTERBLOCKXML{
    const QString BLOCK  = "RTPFilterBlock";
    const QString PARENT = "parent";
    const QString LINKOF = "linkOf";
    const QString ID     = "guid";
}

//! Ключевые слова XML файла для описания величины
namespace DBQUANTITYXML {
    const QString QUANTITY  = "RTPMeasuredEntity";
    const QString BASICUNIT = "basicUnit";
    const QString ALIAS     = "decription";
    const QString ID        = "guid";
    const QString NAME      = "name";
    const QString DIMENSION = "dimensionSymbol";
    const QString PARENT    = "parent";
}

//! Ключевые слова XML файла для описания единиц измерения
namespace DBUNITXML {
    const QString UNIT           = "RTPMeasureUnit";
    const QString CODE           = "code";
    const QString COEFF          = "coeff";
    const QString DELTA          = "delta";
    const QString NAME           = "name";
    const QString ID             = "guid";
    const QString DESIGNATION    = "designation";
    const QString INTDESIGNATION = "intDesignation";
    const QString SYMBOL         = "symbol";
    const QString INTSYMBOL      = "intSymbol";
    const QString PARENT         = "parent";
}

//! Ключевые слова XML файла для описания группы величины
namespace DBQUANTITYGROUPXML {
    const QString QUANTITYGROUP = "RTPEntityGroup";
    const QString NAME          = "name";
    const QString ALIAS         = "description";
    const QString PARENT        = "parent";
    const QString ID            = "guid";
}

//! Ключевые слова XML файла для описания списка классов
namespace DBCLASSLISTXML {
    const QString CLASSLIST = "RTPClassList";
    const QString NAME      = "name";
    const QString ALIAS     = "alias";
    const QString PARENT    = "parent";
    const QString ID        = "guid";
}

//! Ключевые слова XML файла для описания списка единиц измерения
namespace DBQUANTITYLISTXML {
    const QString QUANTITYLIST = "RTPEntityList";
    const QString NAME         = "name";
    const QString ALIAS        = "alias";
    const QString PARENT       = "parent";
    const QString ID           = "guid";
}

//! Ключевые слова XML файла для описания модели
namespace DBMODELXML {
    const QString MODEL  = "RTPModel";
    const QString NAME   = "name";
    const QString ALIAS  = "alias";
    const QString PARENT = "parent";
    const QString ID     = "guid";
}

//! Ключевые слова XML файла для описания списка списков значений
namespace DBLOVLISTXML {
    const QString LOVLIST = "RTPLOVList";
    const QString NAME    = "name";
    const QString ALIAS   = "alias";
    const QString PARENT  = "parent";
    const QString ID      = "guid";
}

//! Ключевые слова XML файла для списка значений
namespace DBLOVXML{
    const QString LOV       = "RTPLOV";
    const QString NAME      = "name";
    const QString ALIAS     = "alias";
    const QString PARENT    = "parent";
    const QString ID        = "guid";
    const QString TYPE      = "type";
    const QString MAXSTRLEN = "maxStringLength";
    const QString ACCURACY  = "accuracy";
}

//! Ключевые слова XML файла для значений списка
namespace DBLOVVALUEXML{
    const QString LOVVALUE = "RTPLOVValue";
    const QString ALIAS     = "name";
    const QString VALUE    = "value";
    const QString PARENT   = "parent";
    const QString ID       = "guid";
}

//! Ключевые слова XML файла для описания списка справочников
namespace DBREFLISTXML {
    const QString REFLIST = "RTPRefList";
    const QString NAME    = "name";
    const QString ALIAS   = "alias";
    const QString PARENT  = "parent";
    const QString ID      = "guid";
}

//! Ключевые слова XML файла для описания групп справочников
namespace DBREFGROUPXML {
    const QString REFGROUP = "RTPRefGroup";
    const QString NAME     = "name";
    const QString ALIAS    = "description";
    const QString PARENT   = "parent";
    const QString ID       = "guid";
}

//! Ключевые слова XML файла для описания справочника
namespace DBREFXML {
    const QString REF    = "RTPRef";
    const QString NAME   = "name";
    const QString ALIAS  = "description";
    const QString PARENT = "parent";
    const QString ID     = "guid";
}

//! Ключевые слова XML файла для описания ссылок на класс
namespace DBLINKTOCLASSXML{
    const QString LINKTOCLASS = "RTPLinkToClass";
    const QString ALIAS       = "alias";
    const QString REFCLASS    = "refClass";
    const QString PARENT      = "parent";
    const QString ID          = "guid";
}

//! Ключевые слова XML файла для описания ссылок на фильтр
namespace DBLINKTOFILTERXML{
    const QString LINKTOFILTER = "RTPLinkToFilter";
    const QString ALIAS        = "alias";
    const QString REFFILTER    = "refFilter";
    const QString PARENT       = "parent";
    const QString ID           = "guid";
}

//! Ключевые слова XML файла для описания ссылок на справочник
namespace DBLINKTOREFXML{
    const QString LINKTOREF = "RTPLinkToRef";
    const QString ALIAS     = "alias";
    const QString REFREF    = "refRef";
    const QString PARENT    = "parent";
    const QString ID        = "guid";
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

//! Уникальность нумератора
namespace DBUNIQUENUMERATORXML{
    const QString UNIQUENUMERATOR = "RTPUniqueNumerator";
    const QString OBJECT          = "Object";
    const QString CLASS           = "Class";
    const QString COMP            = "Composition";
}

//! Ключевые слова XML файла для описания списка нумераторов
namespace DBNUMERATORLISTXML {
    const QString NUMERATORLIST = "RTPNumeratorList";
    const QString NAME          = "name";
    const QString ALIAS         = "alias";
    const QString PARENT        = "parent";
    const QString ID            = "guid";
}

//! Ключевые слова XML файла для нумератора
namespace DBNUMERATORXML{
    const QString NUMERATOR = "RTPNumerator";
    const QString NAME      = "name";
    const QString ALIAS     = "alias";
    const QString STEP      = "step";
    const QString UNIQUE    = "unique";
    const QString PARENT    = "parent";
    const QString ID        = "guid";
}

//! Ключевые слова XML файла для списка нумератора
namespace DBNUMERATORLOVXML{
    const QString NUMERATORLOV = "RTPNumeratorLOV";
    const QString REFLOV       = "referencedLov";
    const QString PARENT       = "parent";
    const QString ID           = "guid";
}

//! Ключевые слова XML файла для регулярного выражения нумератора
namespace DBNUMERATORREGEXXML{
    const QString NUMERATORREGEX = "RTPNumeratorRegEx";
    const QString REGEX          = "regex";
    const QString PARENT         = "parent";
    const QString ID             = "guid";
}

//! Ключевые слова XML файла для GUID
namespace DBSTRUCTGUIDXML{
    const QString STRUCTGUID      = "RTPStructGuid";
    const QString ACCESSMODE      = "6c12460d-ee83-4e8d-b345-0a2b5dee0352";
    const QString CLASS           = "6298e00f-772d-471c-b328-49af7fec61c1";
    const QString CLASSTYPE       = "b14dd4e1-8609-46b3-af52-fae79d024239";
    const QString ATTR            = "a0df1533-eb59-4a0d-b015-36addac14153";
    const QString ATTRTYPE        = "607adfb8-447f-4644-b43f-f55159bc195c";
    const QString LOV             = "9c8299ac-3943-42d1-a6c3-558ca56be19f";
    const QString QUANTITYGROUP   = "05f96912-5d79-440f-850e-01e34fe6a437";
    const QString QUANTITY        = "fb25ee5e-a147-443c-a23a-27bd4e427d3e";
    const QString UNIT            = "d2c7ce48-23de-4078-94a5-ef2dc8e7b3ac";
    const QString NUMERATOR       = "f2a48594-488c-4357-865d-c9fee374ac04";
    const QString NUMERATORLOV    = "5dfb5b26-5e77-4d93-b348-2a631c124bf7";
    const QString NUMERATORREGEX  = "22ee00e7-33c2-4b15-8462-8c9711892253";
    const QString COMP            = "94266979-42b1-494d-8e97-b0dcba2e288f";
    const QString FILTER          = "7e364aca-c022-4f99-bdaa-a729e15eb839";
    const QString BLOCK           = "af63e526-a964-4e16-bba1-e82996c4bc22";
    const QString COND            = "ce7766cf-a990-487f-8d79-87fbb2a10943";
    const QString REFGROUP        = "da58a426-7ac7-436d-8ff4-fc485dcf845c";
    const QString REF             = "cda58a94-251d-4f3d-a0e6-16295d65afa1";
    const QString LINKTOFILTER    = "cde9b27c-4778-4a42-b322-3749befeb652";
    const QString LINKTOREF       = "31a5bd9d-67bf-4c35-a07b-ac545d9b3415";
    const QString LINKTOCLASS     = "3ac56708-d8c4-463d-a7d0-9c78127701c2";
    const QString ROLE            = "9dcda12a-c757-4be3-89a4-fe4381af26af";
    const QString PERMISSION      = "4f7406c5-ff44-4db8-b98a-641f7a6b07dc";
    const QString UNIQUENUMERATOR = "003f03dd-ec93-4afd-ac4c-3bf28ec0fe16";
}

}}

#endif // DBXMLSTRUCT_H
