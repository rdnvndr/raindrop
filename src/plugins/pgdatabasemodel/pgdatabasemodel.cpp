#include "pgdatabasemodel.h"

#include <metadatamodel/dbxmlstruct.h>
#include <QDebug>
#include <QSqlError>

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseModel::PgDatabaseModel(IDatabasePool *pool): IDatabaseModel (pool)
{

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

QString fldBool() {
    return QString("BOOL");
}

QString fldBin(){
    return QString("VARCHAR(4000)");
}

QString fldChar(){
    return QString("CHAR");
}

QString fldDate() {
    return QString();
}

QString fldDcml() {
    return QString();
}

QString fldDbl() {
    return QString("DOUBLE PRECISION");
}

QString fldInt() {
    return QString("INTEGER");
}

QString fldStr(int count, bool variable = true) {
    return QString( (variable) ? "VARCHAR(%1)" : "CHAR(%1)").arg(count);
}

QString fldTime() {
    return QString();
}

QString fldStmp() {
    return QString();
}

QString fldGuid() {
    return QString("CHAR(36)");
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
    QSqlDatabase m_db = QSqlDatabase::database();
    m_db.transaction();
    QSqlQuery query = QSqlQuery(m_db);

    query.exec("CREATE TABLE " + clsTable(DBLOVXML::LOV) + " ("
        + tblField(DBLOVXML::ALIAS)     + " " + fldStr(256) + ","
        + tblField(DBLOVXML::NAME)      + " " + fldStr(27)  + ","
        + tblField(DBLOVXML::TYPE)      + " " + fldStr(15)  + ","
        + tblField(DBLOVXML::MAXSTRLEN) + " " + fldInt()    + ","
        + tblField(DBLOVXML::ACCURACY)  + " " + fldInt()    + ","
        + tblField(DBLOVXML::ID)        + " " + fldGuid()   + " PRIMARY KEY"
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
        + tblField(DBCLASSXML::NAME)     + " " + fldStr(27)        + ","
        + tblField(DBCLASSXML::MODE)     + " " + fldStr(12, false) +
            " REFERENCES " + lovTable(DBACCESSMODEXML::ACCESSMODE) + ","
        + tblField(DBCLASSXML::TYPE)     + " " + fldStr(12, false) +
            " REFERENCES " + lovTable(DBCLASSTYPEXML::CLASSTYPE)   + ","
        + tblField(DBCLASSXML::ALIAS)    + " " + fldStr(256)       + ","
        + tblField(DBCLASSXML::PARENT)   + " " + fldGuid()         +
            " REFERENCES " + clsTable(DBCLASSXML::CLASS)           + ","
        + tblField(DBCLASSXML::TEMPLATE) + " " + fldStr(4000)      + ","
        + tblField(DBCLASSXML::VERCOUNT) + " " + fldInt()          + ","
        + tblField(DBCLASSXML::ICON)     + " " + fldStr(4000)      + ","
        + tblField(DBCLASSXML::ID)       + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBCLASSXML::CLASS, QObject::tr("Класс"), QIcon(),
                DBSTRUCTGUIDXML::CLASS, QString());
    insertClass(query, DBLOVXML::LOV, QObject::tr("Список значений"), QIcon(),
                DBSTRUCTGUIDXML::LOV, QString());

    // Создание описания групп величин
    query.exec("CREATE TABLE " + clsTable(DBQUANTITYGROUPXML::QUANTITYGROUP) + " ("
        + tblField(DBQUANTITYGROUPXML::NAME)   + " " + fldStr(27)  + ","
        + tblField(DBQUANTITYGROUPXML::ALIAS)  + " " + fldStr(256) + ","
//      + tblField(DBQUANTITYGROUPXML::PARENT) + " " + fldGuid()   + ","
        + tblField(DBQUANTITYGROUPXML::ID)     + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBQUANTITYGROUPXML::QUANTITYGROUP,
                QObject::tr("Группа величины"), QIcon(),
                DBSTRUCTGUIDXML::QUANTITYGROUP, QString());



    // Создание описания величин
    query.exec("CREATE TABLE " + clsTable(DBQUANTITYXML::QUANTITY) + " ("
        + tblField(DBQUANTITYXML::ALIAS)     + " " + fldStr(256) + ","
        + tblField(DBQUANTITYXML::NAME)      + " " + fldStr(256) + ","
        + tblField(DBQUANTITYXML::DIMENSION) + " " + fldStr(256) + ","
        + tblField(DBQUANTITYXML::PARENT)    + " " + fldGuid()   +
           " REFERENCES " + clsTable(DBQUANTITYGROUPXML::QUANTITYGROUP) + ","
        + tblField(DBQUANTITYXML::ID)        + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBQUANTITYXML::QUANTITY, QObject::tr("Величина"),
                QIcon(), DBSTRUCTGUIDXML::QUANTITY, QString());


    // Создание описания единиц измерения
    query.exec("CREATE TABLE " + clsTable(DBUNITXML::UNIT) + " ("
        + tblField(DBUNITXML::CODE)           + " " + fldInt()    + ","
        + tblField(DBUNITXML::COEFF)          + " " + fldDbl()    + ","
        + tblField(DBUNITXML::DELTA)          + " " + fldDbl()    + ","
        + tblField(DBUNITXML::NAME)           + " " + fldStr(256) + ","
        + tblField(DBUNITXML::ID)             + " " + fldGuid()   + " PRIMARY KEY,"
        + tblField(DBUNITXML::DESIGNATION)    + " " + fldStr(32)  + ","
        + tblField(DBUNITXML::INTDESIGNATION) + " " + fldStr(32)  + ","
        + tblField(DBUNITXML::SYMBOL)         + " " + fldStr(32)  + ","
        + tblField(DBUNITXML::INTSYMBOL)      + " " + fldStr(32)  + ","
//      + tblField(DBUNITXML::ALIAS)          + " " + fldStr(256) + ","
        + tblField(DBUNITXML::PARENT)         + " " + fldGuid() +
          " REFERENCES " + clsTable(DBQUANTITYXML::QUANTITY) +
    ");");

    query.exec("ALTER TABLE " + clsTable(DBQUANTITYXML::QUANTITY) + " ADD COLUMN "
        + tblField(DBQUANTITYXML::BASICUNIT) + " " + fldGuid() +
          " REFERENCES " + clsTable(DBUNITXML::UNIT) +
    ";");

    insertClass(query, DBUNITXML::UNIT, QObject::tr("Единица измерения"),
                QIcon(), DBSTRUCTGUIDXML::UNIT, QString());

    // Создание описания нумератора
    query.exec("CREATE TABLE " + clsTable(DBNUMERATORXML::NUMERATOR) + " ("
        + tblField(DBNUMERATORXML::ALIAS)  + " " + fldStr(256)       + ","
        + tblField(DBNUMERATORXML::NAME)   + " " + fldStr(27)        + ","
        + tblField(DBNUMERATORXML::STEP)   + " " + fldInt()          + ","
        + tblField(DBNUMERATORXML::UNIQUE) + " " + fldStr(12, false) +
          " REFERENCES " + lovTable(DBUNIQUENUMERATORXML::UNIQUENUMERATOR) + ","
        + tblField(DBNUMERATORXML::PARENT) + " " + fldGuid()         + ","
        + tblField(DBNUMERATORXML::ID)     + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBNUMERATORXML::NUMERATOR, QObject::tr("Нумератор"),
                QIcon(), DBSTRUCTGUIDXML::NUMERATOR, QString());


    // Создание описания списка значений в нумераторе
    query.exec("CREATE TABLE " + clsTable(DBNUMERATORLOVXML::NUMERATORLOV) + " ("
        + tblField(DBNUMERATORLOVXML::REFLOV) + " " + fldGuid() +
          " REFERENCES " + clsTable(DBLOVXML::LOV) + ","
        + tblField(DBNUMERATORLOVXML::PARENT) + " " + fldGuid() +
          " REFERENCES " + clsTable(DBNUMERATORXML::NUMERATOR) + ","
        + tblField(DBNUMERATORLOVXML::ID)     + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBNUMERATORLOVXML::NUMERATORLOV,
                QObject::tr("Список значений нумератора"),
                QIcon(), DBSTRUCTGUIDXML::NUMERATORLOV, QString());


    // Создание описания регулярных выражений в нумераторе
    query.exec("CREATE TABLE " + clsTable(DBNUMERATORREGEXXML::NUMERATORREGEX) + " ("
        + tblField(DBNUMERATORREGEXXML::REGEX)  + " " + fldStr(256) + ","
        + tblField(DBNUMERATORREGEXXML::PARENT) + " " + fldGuid() +
          " REFERENCES " + clsTable(DBNUMERATORXML::NUMERATOR) + ","
        + tblField(DBNUMERATORREGEXXML::ID)     + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBNUMERATORREGEXXML::NUMERATORREGEX,
                QObject::tr("Регулярное выражение нумератора"),
                QIcon(), DBSTRUCTGUIDXML::NUMERATORREGEX, QString());


    // Создание описания атрибутов
    query.exec("CREATE TABLE " + clsTable(DBATTRXML::ATTR) + " ("
        + tblField(DBATTRXML::NAME)           + " " + fldStr(27)   + ","
        + tblField(DBATTRXML::ALIAS)          + " " + fldStr(256)  + ","
        + tblField(DBATTRXML::TYPE)           + " " + fldStr(10, false) +
            " REFERENCES " + lovTable(DBATTRTYPEXML::ATTRTYPE)     + ","
        + tblField(DBATTRXML::GROUP)          + " " + fldStr(256)  + ","
        + tblField(DBATTRXML::MAXSTRLEN)      + " " + fldInt()     + ","
        + tblField(DBATTRXML::ACCURACY)       + " " + fldInt()     + ","
        + tblField(DBATTRXML::ISNULLALLOWED)  + " " + fldBool()    + ","
        + tblField(DBATTRXML::ISUNIQUE)       + " " + fldBool()    + ","
        + tblField(DBATTRXML::ISCANDIDATEKEY) + " " + fldBool()    + ","
        + tblField(DBATTRXML::INITIALVAL)     + " " + fldStr(4000) + ","
        + tblField(DBATTRXML::LOWERBOUND)     + " " + fldStr(4000) + ","
        + tblField(DBATTRXML::UPPERBOUND)     + " " + fldStr(4000) + ","
        + tblField(DBATTRXML::REFCLASS)       + " " + fldGuid() +
          " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
        + tblField(DBATTRXML::REFUNIT)        + " " + fldGuid() +
          " REFERENCES " + clsTable(DBUNITXML::UNIT) + ","
        + tblField(DBATTRXML::REFLOV)         + " " + fldGuid() +
          " REFERENCES " + clsTable(DBLOVXML::LOV) + ","
        + tblField(DBATTRXML::REFNUMERATOR)   + " " + fldGuid() +
          " REFERENCES " + clsTable(DBNUMERATORXML::NUMERATOR) + ","
        + tblField(DBATTRXML::PARENT)         + " " + fldGuid() +
          " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
        + tblField(DBATTRXML::ID)             + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBATTRXML::ATTR, QObject::tr("Атрибут"), QIcon(),
                DBSTRUCTGUIDXML::ATTR, QString());


    // Создание описания состава
    query.exec("CREATE TABLE " + clsTable(DBCOMPXML::COMP) + " ("
        + tblField(DBCOMPXML::LINKCLASS)          + " " + fldGuid() +
          " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
        + tblField(DBCOMPXML::ALIAS)              + " " + fldStr(256) + ","
        + tblField(DBCOMPXML::PARENT)             + " " + fldGuid() +
          " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
        + tblField(DBCOMPXML::CLASS)              + " " + fldGuid()   + ","
        + tblField(DBCOMPXML::ISVIEW)             + " " + fldBool()   + ","
        + tblField(DBCOMPXML::DIRECTDESCRIPTION)  + " " + fldStr(256) + ","
        + tblField(DBCOMPXML::INVERSEDESCRIPTION) + " " + fldStr(256) + ","
        + tblField(DBCOMPXML::ISCOMP)             + " " + fldBool()   + ","
        + tblField(DBCOMPXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBCOMPXML::COMP, QObject::tr("Состав"), QIcon(),
                DBSTRUCTGUIDXML::COMP, QString());

    // Создание описания фильтра
    query.exec("CREATE TABLE " + clsTable(DBFILTERXML::FILTER) + " ("
        + tblField(DBFILTERXML::NAME)               + " " + fldStr(27)  + ","
        + tblField(DBFILTERXML::ALIAS)              + " " + fldStr(256) + ","
        + tblField(DBFILTERXML::PARENT)             + " " + fldGuid() +
          " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
        + tblField(DBFILTERXML::CLASS)              + " " + fldGuid() +
          " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
        + tblField(DBFILTERXML::DIRECTDESCRIPTION)  + " " + fldStr(256) + ","
        + tblField(DBFILTERXML::INVERSEDESCRIPTION) + " " + fldStr(256) + ","
        + tblField(DBFILTERXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBFILTERXML::FILTER, QObject::tr("Фильтр"), QIcon(),
                DBSTRUCTGUIDXML::FILTER, QString());


    // Создание описания блока условий фильтра
    query.exec("CREATE TABLE " + clsTable(DBFILTERBLOCKXML::BLOCK) + " ("
        + tblField(DBFILTERBLOCKXML::PARENT) + " " + fldGuid() +
          " REFERENCES " + clsTable(DBFILTERXML::FILTER) + ","
//      + tblField(DBFILTERBLOCKXML::ALIAS)  + " " + fldStr(256) + ","
        + tblField(DBFILTERBLOCKXML::LINKOF) + " " + fldStr(10)  + ","
        + tblField(DBFILTERBLOCKXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBFILTERBLOCKXML::BLOCK,
                QObject::tr("Блок условий фильтра"), QIcon(),
                DBSTRUCTGUIDXML::BLOCK, QString());


    // Создание описания условия фильтра
    query.exec("CREATE TABLE " + clsTable(DBCONDITIONXML::COND) + " ("
        + tblField(DBCONDITIONXML::FIRSTATTR)  + " " + fldGuid()  +
          " REFERENCES " + clsTable(DBATTRXML::ATTR) + ","
//      + tblField(DBCONDITIONXML::ALIAS)      + " " + fldStr(256) + ","
        + tblField(DBCONDITIONXML::PARENT)     + " " + fldGuid()  +
          " REFERENCES " + clsTable(DBFILTERXML::FILTER) +
          " REFERENCES " + clsTable(DBFILTERBLOCKXML::BLOCK) + ","
        + tblField(DBCONDITIONXML::SECONDATTR) + " " + fldGuid()  +
          " REFERENCES " + clsTable(DBATTRXML::ATTR) + ","
        + tblField(DBCONDITIONXML::OPERATOR)   + " " + fldStr(10) + ","
        + tblField(DBCONDITIONXML::LINKOF)     + " " + fldStr(10) + ","
        + tblField(DBCONDITIONXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBCONDITIONXML::COND,
                QObject::tr("Условие фильтра"), QIcon(),
                DBSTRUCTGUIDXML::COND, QString());


    // Создание описания группы справочников
    query.exec("CREATE TABLE " + clsTable(DBREFGROUPXML::REFGROUP) + " ("
        + tblField(DBREFGROUPXML::ALIAS)  + " " + fldStr(256) + ","
        + tblField(DBREFGROUPXML::NAME)   + " " + fldStr(27)  + ","
        + tblField(DBREFGROUPXML::PARENT) + " " + fldGuid()   + ","
        + tblField(DBREFGROUPXML::ID)     + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBREFGROUPXML::REFGROUP,
                QObject::tr("Группа справочников"), QIcon(),
                DBSTRUCTGUIDXML::REFGROUP, QString());


    // Создание описания справочника
    query.exec("CREATE TABLE " + clsTable(DBREFXML::REF) + " ("
                + tblField(DBREFXML::ALIAS)  + " " + fldStr(256) + ","
                + tblField(DBREFXML::NAME)   + " " + fldStr(27)  + ","
                + tblField(DBREFXML::PARENT) + " " + fldGuid()   +
                    " REFERENCES " + clsTable(DBREFGROUPXML::REFGROUP) + ","
                + tblField(DBREFXML::ID)     + " " + fldGuid() + " PRIMARY KEY"
               ");");

    insertClass(query, DBREFXML::REF,
                QObject::tr("Справочник"), QIcon(),
                DBSTRUCTGUIDXML::REF, QString());


    // Создание описания фильтра справочника
    query.exec("CREATE TABLE " + clsTable(DBLINKTOFILTERXML::LINKTOFILTER) + " ("
        + tblField(DBLINKTOFILTERXML::ALIAS)     + " " + fldStr(256) + ","
        + tblField(DBLINKTOFILTERXML::REFFILTER) + " " + fldGuid()   +
          " REFERENCES " + clsTable(DBFILTERXML::FILTER) + ","
        + tblField(DBLINKTOFILTERXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBLINKTOFILTERXML::LINKTOFILTER,
                QObject::tr("Фильтр справочника"), QIcon(),
                DBSTRUCTGUIDXML::LINKTOFILTER, QString());


    // Создание описания ссылки на справочник
    query.exec("CREATE TABLE " + clsTable(DBLINKTOREFXML::LINKTOREF) + " ("
        + tblField(DBLINKTOREFXML::ALIAS)  + " " + fldStr(256) + ","
        + tblField(DBLINKTOREFXML::REFREF) + " " + fldGuid()   +
          " REFERENCES " + clsTable(DBREFXML::REF) + ","
        + tblField(DBLINKTOREFXML::PARENT) + " " + fldGuid()   +
          " REFERENCES " + clsTable(DBLINKTOFILTERXML::LINKTOFILTER) +
          " REFERENCES " + clsTable(DBREFXML::REF) + ","
        + tblField(DBLINKTOREFXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBLINKTOREFXML::LINKTOREF,
                QObject::tr("Ссылка на справочник"), QIcon(),
                DBSTRUCTGUIDXML::LINKTOREF, QString());


    // Создание описания класса справочника
    query.exec("CREATE TABLE " + clsTable(DBLINKTOCLASSXML::LINKTOCLASS) + " ("
        + tblField(DBLINKTOCLASSXML::ALIAS)    + " " + fldStr(256) + ","
        + tblField(DBLINKTOCLASSXML::REFCLASS) + " " + fldGuid()   +
          " REFERENCES " + clsTable(DBCLASSXML::CLASS) + ","
        + tblField(DBLINKTOCLASSXML::PARENT)    + " " + fldGuid()  +
          " REFERENCES " + clsTable(DBLINKTOFILTERXML::LINKTOFILTER) +
          " REFERENCES " + clsTable(DBREFXML::REF) + ","
        + tblField(DBLINKTOCLASSXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBLINKTOCLASSXML::LINKTOCLASS,
                QObject::tr("Класс справочника"), QIcon(),
                DBSTRUCTGUIDXML::LINKTOCLASS, QString());

    query.exec("ALTER TABLE " + clsTable(DBLINKTOFILTERXML::LINKTOFILTER)
        + " ADD COLUMN "
        + tblField(DBLINKTOFILTERXML::PARENT)  + " " + fldGuid()   +
          " REFERENCES " + clsTable(DBLINKTOCLASSXML::LINKTOCLASS) +
          " REFERENCES " + clsTable(DBLINKTOREFXML::LINKTOREF) +
    ";");


    // Создание описания роли
    query.exec("CREATE TABLE " + clsTable(DBROLEXML::ROLE) + " ("
        + tblField(DBROLEXML::ALIAS)       + " " + fldStr(256)  + ","
        + tblField(DBROLEXML::NAME)        + " " + fldStr(27)   + ","
        + tblField(DBROLEXML::DESCRIPTION) + " " + fldStr(4000) + ","
        + tblField(DBROLEXML::PARENT)      + " " + fldGuid()    + ","
        + tblField(DBROLEXML::ID) + " " + fldGuid() + " PRIMARY KEY"
    ");");

    insertClass(query, DBROLEXML::ROLE,
                QObject::tr("Роль"), QIcon(),
                DBSTRUCTGUIDXML::ROLE, QString());

    // Создание описания прав доступа
    query.exec("CREATE TABLE " + clsTable(DBPERMISSIONXML::PERMISSION) + " ("
        + tblField(DBPERMISSIONXML::ROLE)     + " " + fldGuid() +
          " REFERENCES " + clsTable(DBROLEXML::ROLE)   + ","
        + tblField(DBPERMISSIONXML::ISCREATE) + " " + fldBool() + ","
        + tblField(DBPERMISSIONXML::ISREAD)   + " " + fldBool() + ","
        + tblField(DBPERMISSIONXML::ISWRITE)  + " " + fldBool() + ","
        + tblField(DBPERMISSIONXML::ISDELETE) + " " + fldBool() + ","
        + tblField(DBPERMISSIONXML::ISBLOCK)  + " " + fldBool() + ","
        + tblField(DBPERMISSIONXML::PARENT)   + " " + fldGuid() +
          " REFERENCES " + clsTable(DBCLASSXML::CLASS) +
          " REFERENCES " + clsTable(DBATTRXML::ATTR)   + ","
        + tblField(DBPERMISSIONXML::ID) + " " + fldGuid() + " PRIMARY KEY"
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
