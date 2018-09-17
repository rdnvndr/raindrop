#include "pgdatabasemodel.h"

#include <metadatamodel/dbxmlstruct.h>
#include <QDebug>
#include <QSqlError>

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseModel::PgDatabaseModel(QSqlDatabase db)
{
    m_db = db;
}

QString clsTable(const QString &className) {
    return QString("\"CLS" + className + "\"");
}

QString lovTable(const QString &lovName) {
    return QString("\"LOV" + lovName + "\"");
}

QString tblField(const QString &attrName) {
    return QString("\"" + attrName + "\"");
}

bool insertClass(QSqlQuery &query, const QString &name, const QString &alias,
                 const QIcon icon, const QString &guid, const QString &parent)
{
    query.prepare("INSERT INTO " + clsTable(DBCLASSXML::CLASS) + " ("
                  + tblField(DBCLASSXML::NAME)     + ","
                  + tblField(DBCLASSXML::MODE)     + ","
                  + tblField(DBCLASSXML::TYPE)     + ","
                  + tblField(DBCLASSXML::ALIAS)    + ","
                  + tblField(DBCLASSXML::PARENT)   + ","
                  + tblField(DBCLASSXML::TEMPLATE) + ","
                  + tblField(DBCLASSXML::VERCOUNT) + ","
                  + tblField(DBCLASSXML::ICON)     + ","
                  + tblField(DBCLASSXML::ID)
                + ") VALUES(?,?,?,?,?,?,?,?,?);");
    query.addBindValue(name);
    query.addBindValue(DBACCESSMODEXML::SYSTEM);
    query.addBindValue(DBCLASSTYPEXML::NORMAL);
    query.addBindValue(alias);
    query.addBindValue(parent);
    query.addBindValue(QVariant::String);
    query.addBindValue(0);
    query.addBindValue(icon);
    query.addBindValue(guid);

    return query.exec();
}

bool insertLovValue(QSqlQuery &query, const QString &table,
                    const QString &alias, const QString &value)
{
    query.prepare("INSERT INTO " + lovTable(table) + " ("
                    + tblField(DBLOVVALUEXML::ALIAS) + ","
                    + tblField(DBLOVVALUEXML::VALUE)
               + ") VALUES(?,?);");
    query.addBindValue(alias);
    query.addBindValue(value);

    return query.exec();
}

bool insertLov(QSqlQuery &query, const QString &name,
                    const QString &alias, const QString &guid)
{
    query.exec("CREATE TABLE " + lovTable(name) + " ("
                + tblField(DBLOVVALUEXML::ALIAS)  + " VARCHAR(256),"
                + tblField(DBLOVVALUEXML::VALUE)  + " CHAR(12) PRIMARY KEY"
               ");");

    query.prepare("INSERT INTO " + clsTable(DBLOVXML::LOV) + " ("
                  + tblField(DBLOVXML::ALIAS)     + ","
                  + tblField(DBLOVXML::NAME)      + ","
                  + tblField(DBLOVXML::TYPE)      + ","
                  + tblField(DBLOVXML::MAXSTRLEN) + ","
                  + tblField(DBLOVXML::ACCURACY)  + ","
                  + tblField(DBLOVXML::ID)
               + ") VALUES(?,?,?,?,?,?);");

    query.addBindValue(alias);
    query.addBindValue(name);
    query.addBindValue(DBATTRTYPEXML::STRING);
    query.addBindValue(12);
    query.addBindValue(0);
    query.addBindValue(guid);

    return query.exec();
}

bool PgDatabaseModel::init()
{
    m_db.transaction();
    QSqlQuery query = QSqlQuery(m_db);

    query.exec("CREATE TABLE " + clsTable(DBLOVXML::LOV) + " ("
                + tblField(DBLOVXML::ALIAS)     + " VARCHAR(256),"
                + tblField(DBLOVXML::NAME)      + " VARCHAR(27),"
                + tblField(DBLOVXML::TYPE)      + " VARCHAR(15),"
                + tblField(DBLOVXML::MAXSTRLEN) + " INTEGER,"
                + tblField(DBLOVXML::ACCURACY)  + " INTEGER,"
                + tblField(DBLOVXML::ID)        + " CHAR(36) PRIMARY KEY"
               ");");


    insertLov(query, DBATTRTYPEXML::ATTRTYPE, QObject::tr("Тип атрибута"),
              DBSTRUCTGUIDXML::ATTRTYPE);

    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::BOOLEAN, DBATTRTYPEXML::BOOLEAN);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::BINARY, DBATTRTYPEXML::BINARY);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::CHAR, DBATTRTYPEXML::CHAR);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::DATE, DBATTRTYPEXML::DATE);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::DECIMAL, DBATTRTYPEXML::DECIMAL);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::DOUBLE, DBATTRTYPEXML::DOUBLE);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::INTEGER, DBATTRTYPEXML::INTEGER);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::STRING, DBATTRTYPEXML::STRING);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::REFERENCE, DBATTRTYPEXML::REFERENCE);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::TIME, DBATTRTYPEXML::TIME);
    insertLovValue(query, DBATTRTYPEXML::ATTRTYPE,
                   DBATTRTYPEXML::TIMESHTAMP, DBATTRTYPEXML::TIMESHTAMP);


    insertLov(query, DBCLASSTYPEXML::CLASSTYPE, QObject::tr("Тип класса"),
              DBSTRUCTGUIDXML::CLASSTYPE);
    insertLovValue(query, DBCLASSTYPEXML::CLASSTYPE,
                   DBCLASSTYPEXML::ABSTRACT, DBCLASSTYPEXML::ABSTRACT);
    insertLovValue(query, DBCLASSTYPEXML::CLASSTYPE,
                   DBCLASSTYPEXML::CONTEXT, DBCLASSTYPEXML::CONTEXT);
    insertLovValue(query, DBCLASSTYPEXML::CLASSTYPE,
                   DBCLASSTYPEXML::EMBEDDED, DBCLASSTYPEXML::EMBEDDED);
    insertLovValue(query, DBCLASSTYPEXML::CLASSTYPE,
                   DBCLASSTYPEXML::NORMAL, DBCLASSTYPEXML::NORMAL);


    insertLov(query, DBACCESSMODEXML::ACCESSMODE, QObject::tr("Режим доступа"),
              DBSTRUCTGUIDXML::ACCESSMODE);
    insertLovValue(query, DBACCESSMODEXML::ACCESSMODE,
                   DBACCESSMODEXML::STANDART, DBACCESSMODEXML::STANDART);
    insertLovValue(query, DBACCESSMODEXML::ACCESSMODE,
                   DBACCESSMODEXML::SYSTEM, DBACCESSMODEXML::SYSTEM);
    insertLovValue(query, DBACCESSMODEXML::ACCESSMODE,
                   DBACCESSMODEXML::USER, DBACCESSMODEXML::USER);


    insertLov(query, DBUNIQUENUMERATORXML::UNIQUENUMERATOR,
              QObject::tr("Уникальность нумератора"),
              DBSTRUCTGUIDXML::UNIQUENUMERATOR);
    insertLovValue(query, DBUNIQUENUMERATORXML::UNIQUENUMERATOR,
                   DBUNIQUENUMERATORXML::CLASS, DBUNIQUENUMERATORXML::CLASS);
    insertLovValue(query, DBUNIQUENUMERATORXML::UNIQUENUMERATOR,
                   DBUNIQUENUMERATORXML::COMP, DBUNIQUENUMERATORXML::COMP);
    insertLovValue(query, DBUNIQUENUMERATORXML::UNIQUENUMERATOR,
                   DBUNIQUENUMERATORXML::OBJECT, DBUNIQUENUMERATORXML::OBJECT);


    // Создание описания классов
    query.exec("CREATE TABLE " + clsTable(DBCLASSXML::CLASS) + " ("
                + tblField(DBCLASSXML::NAME)     + " VARCHAR(27),"
                + tblField(DBCLASSXML::MODE)     + " CHAR(12)"
                    " REFERENCES " + lovTable(DBACCESSMODEXML::ACCESSMODE) + ","
                + tblField(DBCLASSXML::TYPE)     + " CHAR(12)"
                    " REFERENCES " + lovTable(DBCLASSTYPEXML::CLASSTYPE) + ","
                + tblField(DBCLASSXML::ALIAS)    + " VARCHAR(256),"
                + tblField(DBCLASSXML::PARENT)   + " CHAR(36)"
                    " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                + tblField(DBCLASSXML::TEMPLATE) + " VARCHAR(4000),"
                + tblField(DBCLASSXML::VERCOUNT) + " INTEGER,"
                + tblField(DBCLASSXML::ICON)     + " VARCHAR(4000),"
                + tblField(DBCLASSXML::ID)       + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBCLASSXML::CLASS, QObject::tr("Класс"), QIcon(),
                DBSTRUCTGUIDXML::CLASS, QString());
    insertClass(query, DBLOVXML::LOV, QObject::tr("Список значений"), QIcon(),
                DBSTRUCTGUIDXML::LOV, QString());

    // Создание описания групп величин
    query.exec("CREATE TABLE " + clsTable(DBQUANTITYGROUPXML::QUANTITYGROUP) + " ("
                + tblField(DBQUANTITYGROUPXML::NAME)   + " VARCHAR(27),"
                + tblField(DBQUANTITYGROUPXML::ALIAS)  + " VARCHAR(256),"
//                + tblField(DBQUANTITYGROUPXML::PARENT) + " CHAR(36),"
                + tblField(DBQUANTITYGROUPXML::ID)     + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBQUANTITYGROUPXML::QUANTITYGROUP,
                QObject::tr("Группа величины"), QIcon(),
                DBSTRUCTGUIDXML::QUANTITYGROUP, QString());



    // Создание описания величин
    query.exec("CREATE TABLE " + clsTable(DBQUANTITYXML::QUANTITY) + " ("
            + tblField(DBQUANTITYXML::ALIAS)     + " VARCHAR(256),"
            + tblField(DBQUANTITYXML::NAME)      + " VARCHAR(256),"
            + tblField(DBQUANTITYXML::DIMENSION) + " VARCHAR(256),"
            + tblField(DBQUANTITYXML::PARENT)    + " CHAR(36)"
              " REFERENCES " + clsTable(DBQUANTITYGROUPXML::QUANTITYGROUP) + ","
            + tblField(DBQUANTITYXML::ID)        + " CHAR(36) PRIMARY KEY"
        ");");

    insertClass(query, DBQUANTITYXML::QUANTITY, QObject::tr("Величина"),
                QIcon(), DBSTRUCTGUIDXML::QUANTITY, QString());


    // Создание описания единиц измерения
    query.exec("CREATE TABLE " + clsTable(DBUNITXML::UNIT) + " ("
                + tblField(DBUNITXML::CODE)           + " INTEGER,"
                + tblField(DBUNITXML::COEFF)          + " DOUBLE PRECISION,"
                + tblField(DBUNITXML::DELTA)          + " DOUBLE PRECISION,"
                + tblField(DBUNITXML::NAME)           + " VARCHAR(256),"
                + tblField(DBUNITXML::ID)             + " CHAR(36) PRIMARY KEY,"
                + tblField(DBUNITXML::DESIGNATION)    + " VARCHAR(32),"
                + tblField(DBUNITXML::INTDESIGNATION) + " VARCHAR(32),"
                + tblField(DBUNITXML::SYMBOL)         + " VARCHAR(32),"
                + tblField(DBUNITXML::INTSYMBOL)      + " VARCHAR(32),"
//                + tblField(DBUNITXML::ALIAS)          + " VARCHAR(256),"
                + tblField(DBUNITXML::PARENT)         + " CHAR(36)"
                    " REFERENCES \"CLS" + DBQUANTITYXML::QUANTITY
              + ");");

    query.exec("ALTER TABLE " + clsTable(DBQUANTITYXML::QUANTITY) + " ADD COLUMN "
                + tblField(DBQUANTITYXML::BASICUNIT) + " CHAR(36)"
                    " REFERENCES " + clsTable(DBUNITXML::UNIT)
               + ";");

    insertClass(query, DBUNITXML::UNIT, QObject::tr("Единица измерения"),
                QIcon(), DBSTRUCTGUIDXML::UNIT, QString());

    // Создание описания нумератора
    query.exec("CREATE TABLE " + clsTable(DBNUMERATORXML::NUMERATOR) + " ("
                + tblField(DBNUMERATORXML::ALIAS)  + " VARCHAR(256),"
                + tblField(DBNUMERATORXML::NAME)   + " VARCHAR(27),"
                + tblField(DBNUMERATORXML::STEP)   + " INTEGER,"
                + tblField(DBNUMERATORXML::UNIQUE) + " CHAR(12)"
                    " REFERENCES " + lovTable(DBUNIQUENUMERATORXML::UNIQUENUMERATOR) + ","
                + tblField(DBNUMERATORXML::PARENT) + " CHAR(36),"
                + tblField(DBNUMERATORXML::ID)     + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBNUMERATORXML::NUMERATOR, QObject::tr("Нумератор"),
                QIcon(), DBSTRUCTGUIDXML::NUMERATOR, QString());


    // Создание описания списка значений в нумераторе
    query.exec("CREATE TABLE " + clsTable(DBNUMERATORLOVXML::NUMERATORLOV) + " ("
                + tblField(DBNUMERATORLOVXML::REFLOV) + " CHAR(36)"
                    " REFERENCES " + clsTable(DBLOVXML::LOV) + ","
                + tblField(DBNUMERATORLOVXML::PARENT) + " CHAR(36)"
                    " REFERENCES " + clsTable(DBNUMERATORXML::NUMERATOR) + ","
                + tblField(DBNUMERATORLOVXML::ID)     + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBNUMERATORLOVXML::NUMERATORLOV,
                QObject::tr("Список значений нумератора"),
                QIcon(), DBSTRUCTGUIDXML::NUMERATORLOV, QString());


    // Создание описания регулярных выражений в нумераторе
    query.exec("CREATE TABLE " + clsTable(DBNUMERATORREGEXXML::NUMERATORREGEX) + " ("
                + tblField(DBNUMERATORREGEXXML::REGEX)  + " VARCHAR(256),"
                + tblField(DBNUMERATORREGEXXML::PARENT) + " CHAR(36)"
                    " REFERENCES " + clsTable(DBNUMERATORXML::NUMERATOR) + ","
                + tblField(DBNUMERATORREGEXXML::ID)     + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBNUMERATORREGEXXML::NUMERATORREGEX,
                QObject::tr("Регулярное выражение нумератора"),
                QIcon(), DBSTRUCTGUIDXML::NUMERATORREGEX, QString());


    // Создание описания атрибутов
    query.exec("CREATE TABLE " + clsTable(DBATTRXML::ATTR) + " ("
                + tblField(DBATTRXML::NAME)           + " VARCHAR(27),"
                + tblField(DBATTRXML::ALIAS)          + " VARCHAR(256),"
                + tblField(DBATTRXML::TYPE)           + " CHAR(10)"
                    " REFERENCES " + lovTable(DBATTRTYPEXML::ATTRTYPE) + ","
                + tblField(DBATTRXML::GROUP)          + " VARCHAR(256),"
                + tblField(DBATTRXML::MAXSTRLEN)      + " INTEGER,"
                + tblField(DBATTRXML::ACCURACY)       + " INTEGER,"
                + tblField(DBATTRXML::ISNULLALLOWED)  + " BOOL,"
                + tblField(DBATTRXML::ISUNIQUE)       + " BOOL,"
                + tblField(DBATTRXML::ISCANDIDATEKEY) + " BOOL,"
                + tblField(DBATTRXML::INITIALVAL)     + " VARCHAR(4000),"
                + tblField(DBATTRXML::LOWERBOUND)     + " VARCHAR(4000),"
                + tblField(DBATTRXML::UPPERBOUND)     + " VARCHAR(4000),"
                + tblField(DBATTRXML::REFCLASS)       + " CHAR(36)"
                    " REFERENCES " + lovTable(DBCLASSXML::CLASS) + ","
                + tblField(DBATTRXML::REFUNIT)        + " CHAR(36)"
                    " REFERENCES " + lovTable(DBUNITXML::UNIT) + ","
                + tblField(DBATTRXML::REFLOV)         + " CHAR(36)"
                    " REFERENCES " + lovTable(DBLOVXML::LOV) + ","
                + tblField(DBATTRXML::REFNUMERATOR)   + " CHAR(36)"
                    " REFERENCES " + lovTable(DBNUMERATORXML::NUMERATOR) + ","
                + tblField(DBATTRXML::PARENT)         + " CHAR(36)"
                    " REFERENCES " + lovTable(DBCLASSXML::CLASS) + ","
                + tblField(DBATTRXML::ID)             + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBATTRXML::ATTR, QObject::tr("Атрибут"), QIcon(),
                DBSTRUCTGUIDXML::ATTR, QString());


    // Создание описания состава
    query.exec("CREATE TABLE " + clsTable(DBCOMPXML::COMP) + " ("
                + tblField(DBCOMPXML::LINKCLASS)          + " CHAR(36)"
                    " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                + tblField(DBCOMPXML::ALIAS)              + " VARCHAR(256),"
                + tblField(DBCOMPXML::PARENT)             + " CHAR(36)"
                    " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                + tblField(DBCOMPXML::CLASS)              + " CHAR(36),"
                + tblField(DBCOMPXML::ISVIEW)             + " BOOL,"
                + tblField(DBCOMPXML::DIRECTDESCRIPTION)  + " VARCHAR(256),"
                + tblField(DBCOMPXML::INVERSEDESCRIPTION) + " VARCHAR(256),"
                + tblField(DBCOMPXML::ISCOMP)             + " BOOL,"
                + tblField(DBCOMPXML::ID)                 + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBCOMPXML::COMP, QObject::tr("Состав"), QIcon(),
                DBSTRUCTGUIDXML::COMP, QString());

    // Создание описания фильтра
    query.exec("CREATE TABLE " + clsTable(DBFILTERXML::FILTER) + " ("
                + tblField(DBFILTERXML::NAME)               + " VARCHAR(27),"
                + tblField(DBFILTERXML::ALIAS)              + " VARCHAR(256),"
                + tblField(DBFILTERXML::PARENT)             + " CHAR(36)"
                    " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                + tblField(DBFILTERXML::CLASS)              + " CHAR(36)"
                    " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                + tblField(DBFILTERXML::DIRECTDESCRIPTION)  + " VARCHAR(256),"
                + tblField(DBFILTERXML::INVERSEDESCRIPTION) + " VARCHAR(256),"
                + tblField(DBFILTERXML::ID)                 + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBFILTERXML::FILTER, QObject::tr("Фильтр"), QIcon(),
                DBSTRUCTGUIDXML::FILTER, QString());


    // Создание описания блока условий фильтра
    query.exec("CREATE TABLE " + clsTable(DBFILTERBLOCKXML::BLOCK) + " ("
                + tblField(DBFILTERBLOCKXML::PARENT) + " CHAR(36)"
                    " REFERENCES " + clsTable(DBFILTERXML::FILTER) + ","
//                + tblField(DBFILTERBLOCKXML::ALIAS)  + " VARCHAR(256),"
                + tblField(DBFILTERBLOCKXML::LINKOF) + " VARCHAR(10),"
                + tblField(DBFILTERBLOCKXML::ID)     + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBFILTERBLOCKXML::BLOCK,
                QObject::tr("Блок условий фильтра"), QIcon(),
                DBSTRUCTGUIDXML::BLOCK, QString());


    // Создание описания условия фильтра
    query.exec("CREATE TABLE " + clsTable(DBCONDITIONXML::COND) + " ("
                + tblField(DBCONDITIONXML::FIRSTATTR)  + " CHAR(36)"
                    " REFERENCES " + clsTable(DBATTRXML::ATTR) + ","
//                + tblField(DBCONDITIONXML::ALIAS)      + " VARCHAR(256),"
                + tblField(DBCONDITIONXML::PARENT)     + " CHAR(36)"
                    " REFERENCES " + clsTable(DBFILTERXML::FILTER) +
                    " REFERENCES " + clsTable(DBFILTERBLOCKXML::BLOCK) + ","
                + tblField(DBCONDITIONXML::SECONDATTR) + " CHAR(36)"
                    " REFERENCES " + clsTable(DBATTRXML::ATTR) + ","
                + tblField(DBCONDITIONXML::OPERATOR)   + " VARCHAR(10),"
                + tblField(DBCONDITIONXML::LINKOF)     + " VARCHAR(10),"
                + tblField(DBCONDITIONXML::ID)         + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBCONDITIONXML::COND,
                QObject::tr("Условие фильтра"), QIcon(),
                DBSTRUCTGUIDXML::COND, QString());


    // Создание описания группы справочников
    query.exec("CREATE TABLE " + clsTable(DBREFGROUPXML::REFGROUP) + " ("
                + tblField(DBREFGROUPXML::ALIAS)  + " VARCHAR(256),"
                + tblField(DBREFGROUPXML::NAME)   + " VARCHAR(27),"
                + tblField(DBREFGROUPXML::PARENT) + " CHAR(36),"
                + tblField(DBREFGROUPXML::ID)     + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBREFGROUPXML::REFGROUP,
                QObject::tr("Группа справочников"), QIcon(),
                DBSTRUCTGUIDXML::REFGROUP, QString());


    // Создание описания справочника
    query.exec("CREATE TABLE " + clsTable(DBREFXML::REF) + " ("
                + tblField(DBREFXML::ALIAS)  + " VARCHAR(256),"
                + tblField(DBREFXML::NAME)   + " VARCHAR(27),"
                + tblField(DBREFXML::PARENT) + " CHAR(36)"
                    " REFERENCES " + clsTable(DBREFGROUPXML::REFGROUP) + ","
                + tblField(DBREFXML::ID)     + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBREFXML::REF,
                QObject::tr("Справочник"), QIcon(),
                DBSTRUCTGUIDXML::REF, QString());


    // Создание описания фильтра справочника
    query.exec("CREATE TABLE " + clsTable(DBLINKTOFILTERXML::LINKTOFILTER) + " ("
                + tblField(DBLINKTOFILTERXML::ALIAS)     + " VARCHAR(256),"
                + tblField(DBLINKTOFILTERXML::REFFILTER) + " CHAR(36)"
                    " REFERENCES " + clsTable(DBFILTERXML::FILTER) + ","
                + tblField(DBLINKTOFILTERXML::ID)        + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBLINKTOFILTERXML::LINKTOFILTER,
                QObject::tr("Фильтр справочника"), QIcon(),
                DBSTRUCTGUIDXML::LINKTOFILTER, QString());


    // Создание описания ссылки на справочник
    query.exec("CREATE TABLE " + clsTable(DBLINKTOREFXML::LINKTOREF) + " ("
                + tblField(DBLINKTOREFXML::ALIAS)  + " VARCHAR(256),"
                + tblField(DBLINKTOREFXML::REFREF) + " CHAR(36)"
                    " REFERENCES " + clsTable(DBREFXML::REF) + ","
                + tblField(DBLINKTOREFXML::PARENT) + " CHAR(36)"
                    " REFERENCES " + clsTable(DBLINKTOFILTERXML::LINKTOFILTER) +
                    " REFERENCES " + clsTable(DBREFXML::REF) + ","
                + tblField(DBLINKTOREFXML::ID)     + " CHAR(36) PRIMARY KEY"
            ");");

    insertClass(query, DBLINKTOREFXML::LINKTOREF,
                QObject::tr("Ссылка на справочник"), QIcon(),
                DBSTRUCTGUIDXML::LINKTOREF, QString());


    // Создание описания класса справочника
    query.exec("CREATE TABLE " + clsTable(DBLINKTOCLASSXML::LINKTOCLASS) + " ("
                + tblField(DBLINKTOCLASSXML::ALIAS)    + " VARCHAR(256),"
                + tblField(DBLINKTOCLASSXML::REFCLASS) + " CHAR(36)"
                    " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
                + tblField(DBLINKTOCLASSXML::PARENT)   + " CHAR(36)"
                    " REFERENCES " + clsTable(DBLINKTOFILTERXML::LINKTOFILTER) +
                    " REFERENCES " + clsTable(DBREFXML::REF) + ","
                + tblField(DBLINKTOCLASSXML::ID)       + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBLINKTOCLASSXML::LINKTOCLASS,
                QObject::tr("Класс справочника"), QIcon(),
                DBSTRUCTGUIDXML::LINKTOCLASS, QString());

    query.exec("ALTER TABLE " + clsTable(DBLINKTOFILTERXML::LINKTOFILTER)
               + " ADD COLUMN "
               + tblField(DBLINKTOFILTERXML::PARENT) + " CHAR(36)"
                 " REFERENCES " + clsTable(DBLINKTOCLASSXML::LINKTOCLASS) +
                 " REFERENCES " + clsTable(DBLINKTOREFXML::LINKTOREF) +
               ";");


    // Создание описания роли
    query.exec("CREATE TABLE " + clsTable(DBROLEXML::ROLE) + " ("
                + tblField(DBROLEXML::ALIAS)       + " VARCHAR(256),"
                + tblField(DBROLEXML::NAME)        + " VARCHAR(27),"
                + tblField(DBROLEXML::DESCRIPTION) + " VARCHAR(4000),"
                + tblField(DBROLEXML::PARENT)      + " CHAR(36),"
                + tblField(DBROLEXML::ID)          + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBROLEXML::ROLE,
                QObject::tr("Роль"), QIcon(),
                DBSTRUCTGUIDXML::ROLE, QString());

    // Создание описания прав доступа
    query.exec("CREATE TABLE " + clsTable(DBPERMISSIONXML::PERMISSION) + " ("
                + tblField(DBPERMISSIONXML::ROLE)     + " CHAR(36)"
                    " REFERENCES " + clsTable(DBROLEXML::ROLE)   + ","
                + tblField(DBPERMISSIONXML::ISCREATE) + " BOOL,"
                + tblField(DBPERMISSIONXML::ISREAD)   + " BOOL,"
                + tblField(DBPERMISSIONXML::ISWRITE)  + " BOOL,"
                + tblField(DBPERMISSIONXML::ISDELETE) + " BOOL,"
                + tblField(DBPERMISSIONXML::ISBLOCK)  + " BOOL,"
                + tblField(DBPERMISSIONXML::PARENT)   + " CHAR(36)"
                    " REFERENCES " + clsTable(DBCLASSXML::CLASS) +
                    " REFERENCES " + clsTable(DBATTRXML::ATTR)   + ","
                + tblField(DBPERMISSIONXML::ID)       + " CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBPERMISSIONXML::PERMISSION,
                QObject::tr("Право доступа"), QIcon(),
                DBSTRUCTGUIDXML::PERMISSION, QString());

    m_db.commit();
    return true;
}

IDatabaseClass *PgDatabaseModel::createClass(
        const QString &name, const QString &base)
{
    Q_UNUSED(name)
    Q_UNUSED(base)

    return nullptr;
}

IDatabaseClass *PgDatabaseModel::oneClass(const QString &name)
{
    Q_UNUSED(name)

    return nullptr;
}

IDatabaseClasses *PgDatabaseModel::classList()
{
    return nullptr;
}

IDatabaseClasses *PgDatabaseModel::derivedClassList(const QString &base)
{
    Q_UNUSED(base)

    return nullptr;
}

}}
