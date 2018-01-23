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

bool insertClass(QSqlQuery &query, const QString &name, const QString &alias,
                 const QIcon icon, const QString &guid, const QString &parent)
{
    query.prepare("INSERT INTO \"CLS" + DBCLASSXML::CLASS + "\"("
                  "\"" + DBCLASSXML::NAME     + "\","
                  "\"" + DBCLASSXML::MODE     + "\","
                  "\"" + DBCLASSXML::TYPE     + "\","
                  "\"" + DBCLASSXML::ALIAS    + "\","
                  "\"" + DBCLASSXML::PARENT   + "\","
                  "\"" + DBCLASSXML::TEMPLATE + "\","
                  "\"" + DBCLASSXML::VERCOUNT + "\","
                  "\"" + DBCLASSXML::ICON     + "\","
                  "\"" + DBCLASSXML::ID       + "\""
                  ") VALUES(?,?,?,?,?,?,?,?,?);");
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
    query.prepare("INSERT INTO \"LOV" + table + "\"("
                    "\"" + DBLOVVALUEXML::ALIAS + "\","
                    "\"" + DBLOVVALUEXML::VALUE + "\""
               ") VALUES(?,?);");
    query.addBindValue(alias);
    query.addBindValue(value);

    return query.exec();
}

bool insertLov(QSqlQuery &query, const QString &name,
                    const QString &alias, const QString &guid)
{
    query.exec("CREATE TABLE \"LOV" + name + "\" ("
               "\"" + DBLOVVALUEXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBLOVVALUEXML::VALUE  + "\" CHAR(12) PRIMARY KEY"
               ");");

    query.prepare("INSERT INTO \"CLS" + DBLOVXML::LOV + "\"("
                    "\"" + DBLOVXML::ALIAS     + "\","
                    "\"" + DBLOVXML::NAME      + "\","
                    "\"" + DBLOVXML::TYPE      + "\","
                    "\"" + DBLOVXML::MAXSTRLEN + "\","
                    "\"" + DBLOVXML::ACCURACY  + "\","
                    "\"" + DBLOVXML::ID        + "\""
               ") VALUES(?,?,?,?,?,?);");

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

    query.exec("CREATE TABLE \"CLS" + DBLOVXML::LOV + "\" ("
               "\"" + DBLOVXML::ALIAS     + "\" VARCHAR(256),"
               "\"" + DBLOVXML::NAME      + "\" VARCHAR(27),"
               "\"" + DBLOVXML::TYPE      + "\" VARCHAR(15),"
               "\"" + DBLOVXML::MAXSTRLEN + "\" INTEGER,"
               "\"" + DBLOVXML::ACCURACY  + "\" INTEGER,"
               "\"" + DBLOVXML::ID        + "\" CHAR(36) PRIMARY KEY"
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
    query.exec("CREATE TABLE \"CLS" + DBCLASSXML::CLASS + "\" ("
               "\"" + DBCLASSXML::NAME     + "\" VARCHAR(27),"
               "\"" + DBCLASSXML::MODE     + "\" CHAR(12)"
                    " REFERENCES \"LOV" + DBACCESSMODEXML::ACCESSMODE + "\","
               "\"" + DBCLASSXML::TYPE     + "\" CHAR(12)"
                    " REFERENCES \"LOV" + DBCLASSTYPEXML::CLASSTYPE + "\","
               "\"" + DBCLASSXML::ALIAS    + "\" VARCHAR(256),"
               "\"" + DBCLASSXML::PARENT   + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBCLASSXML::CLASS + "\","
               "\"" + DBCLASSXML::TEMPLATE + "\" VARCHAR(4000),"
               "\"" + DBCLASSXML::VERCOUNT + "\" INTEGER,"
               "\"" + DBCLASSXML::ICON     + "\" VARCHAR(4000),"
               "\"" + DBCLASSXML::ID       + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBCLASSXML::CLASS, QObject::tr("Класс"), QIcon(),
                DBSTRUCTGUIDXML::CLASS, QString());
    insertClass(query, DBLOVXML::LOV, QObject::tr("Список значений"), QIcon(),
                DBSTRUCTGUIDXML::LOV, QString());

    // Создание описания групп величин
    query.exec("CREATE TABLE \"CLS" + DBQUANTITYGROUPXML::QUANTITYGROUP + "\" ("
               "\"" + DBQUANTITYGROUPXML::NAME   + "\" VARCHAR(27),"
               "\"" + DBQUANTITYGROUPXML::ALIAS  + "\" VARCHAR(256),"
//               "\"" + DBQUANTITYGROUPXML::PARENT + "\" CHAR(36),"
               "\"" + DBQUANTITYGROUPXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBQUANTITYGROUPXML::QUANTITYGROUP,
                QObject::tr("Группа величины"), QIcon(),
                DBSTRUCTGUIDXML::QUANTITYGROUP, QString());



    // Создание описания величин
    query.exec("CREATE TABLE \"CLS" + DBQUANTITYXML::QUANTITY + "\" ("
               "\"" + DBQUANTITYXML::ALIAS     + "\" VARCHAR(256),"
               "\"" + DBQUANTITYXML::NAME      + "\" VARCHAR(256),"
               "\"" + DBQUANTITYXML::DIMENSION + "\" VARCHAR(256),"
               "\"" + DBQUANTITYXML::PARENT    + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBQUANTITYGROUPXML::QUANTITYGROUP + "\","
               "\"" + DBQUANTITYXML::ID        + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBQUANTITYXML::QUANTITY, QObject::tr("Величина"),
                QIcon(), DBSTRUCTGUIDXML::QUANTITY, QString());


    // Создание описания единиц измерения
    query.exec("CREATE TABLE \"CLS" + DBUNITXML::UNIT + "\" ("
               "\"" + DBUNITXML::CODE           + "\" INTEGER,"
               "\"" + DBUNITXML::COEFF          + "\" DOUBLE PRECISION,"
               "\"" + DBUNITXML::DELTA          + "\" DOUBLE PRECISION,"
               "\"" + DBUNITXML::NAME           + "\" VARCHAR(256),"
               "\"" + DBUNITXML::ID             + "\" CHAR(36) PRIMARY KEY,"
               "\"" + DBUNITXML::DESIGNATION    + "\" VARCHAR(32),"
               "\"" + DBUNITXML::INTDESIGNATION + "\" VARCHAR(32),"
               "\"" + DBUNITXML::SYMBOL         + "\" VARCHAR(32),"
               "\"" + DBUNITXML::INTSYMBOL      + "\" VARCHAR(32),"
//               "\"" + DBUNITXML::ALIAS          + "\" VARCHAR(256),"
               "\"" + DBUNITXML::PARENT         + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBQUANTITYXML::QUANTITY + "\""
               ");");

    query.exec("ALTER TABLE \"CLS" + DBQUANTITYXML::QUANTITY + "\" ADD COLUMN "
               "\"" + DBQUANTITYXML::BASICUNIT + "\" CHAR(36)"
               " REFERENCES \"CLS" + DBUNITXML::UNIT + "\""
               ";");

    insertClass(query, DBUNITXML::UNIT, QObject::tr("Единица измерения"),
                QIcon(), DBSTRUCTGUIDXML::UNIT, QString());

    // Создание описания нумератора
    query.exec("CREATE TABLE \"CLS" + DBNUMERATORXML::NUMERATOR + "\" ("
               "\"" + DBNUMERATORXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBNUMERATORXML::NAME   + "\" VARCHAR(27),"
               "\"" + DBNUMERATORXML::STEP   + "\" INTEGER,"
               "\"" + DBNUMERATORXML::UNIQUE + "\" CHAR(12)"
                    " REFERENCES \"LOV" + DBUNIQUENUMERATORXML::UNIQUENUMERATOR + "\","
               "\"" + DBNUMERATORXML::PARENT + "\" CHAR(36),"
               "\"" + DBNUMERATORXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBNUMERATORXML::NUMERATOR, QObject::tr("Нумератор"),
                QIcon(), DBSTRUCTGUIDXML::NUMERATOR, QString());


    // Создание описания списка значений в нумераторе
    query.exec("CREATE TABLE \"CLS" + DBNUMERATORLOVXML::NUMERATORLOV + "\" ("
               "\"" + DBNUMERATORLOVXML::REFLOV + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBLOVXML::LOV + "\","
               "\"" + DBNUMERATORLOVXML::PARENT + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBNUMERATORXML::NUMERATOR + "\","
               "\"" + DBNUMERATORLOVXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBNUMERATORLOVXML::NUMERATORLOV,
                QObject::tr("Список значений нумератора"),
                QIcon(), DBSTRUCTGUIDXML::NUMERATORLOV, QString());


    // Создание описания регулярных выражений в нумераторе
    query.exec("CREATE TABLE \"CLS" + DBNUMERATORREGEXXML::NUMERATORREGEX + "\" ("
               "\"" + DBNUMERATORREGEXXML::REGEX  + "\" VARCHAR(256),"
               "\"" + DBNUMERATORREGEXXML::PARENT + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBNUMERATORXML::NUMERATOR + "\","
               "\"" + DBNUMERATORREGEXXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBNUMERATORREGEXXML::NUMERATORREGEX,
                QObject::tr("Регулярное выражение нумератора"),
                QIcon(), DBSTRUCTGUIDXML::NUMERATORREGEX, QString());


    // Создание описания атрибутов
    query.exec("CREATE TABLE \"CLS" + DBATTRXML::ATTR + "\" ("
               "\"" + DBATTRXML::NAME           + "\" VARCHAR(27),"
               "\"" + DBATTRXML::ALIAS          + "\" VARCHAR(256),"
               "\"" + DBATTRXML::TYPE           + "\" CHAR(10)"
                    " REFERENCES \"LOV" + DBATTRTYPEXML::ATTRTYPE + "\","
               "\"" + DBATTRXML::GROUP          + "\" VARCHAR(256),"
               "\"" + DBATTRXML::MAXSTRLEN      + "\" INTEGER,"
               "\"" + DBATTRXML::ACCURACY       + "\" INTEGER,"
               "\"" + DBATTRXML::ISNULLALLOWED  + "\" BOOL,"
               "\"" + DBATTRXML::ISUNIQUE       + "\" BOOL,"
               "\"" + DBATTRXML::ISCANDIDATEKEY + "\" BOOL,"
               "\"" + DBATTRXML::INITIALVAL     + "\" VARCHAR(4000),"
               "\"" + DBATTRXML::LOWERBOUND     + "\" VARCHAR(4000),"
               "\"" + DBATTRXML::UPPERBOUND     + "\" VARCHAR(4000),"
               "\"" + DBATTRXML::REFCLASS       + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBCLASSXML::CLASS + "\","
               "\"" + DBATTRXML::REFUNIT        + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBUNITXML::UNIT + "\","
               "\"" + DBATTRXML::REFLOV         + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBLOVXML::LOV + "\","
               "\"" + DBATTRXML::REFNUMERATOR   + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBNUMERATORXML::NUMERATOR + "\","
               "\"" + DBATTRXML::PARENT         + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBCLASSXML::CLASS + "\","
               "\"" + DBATTRXML::ID             + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBATTRXML::ATTR, QObject::tr("Атрибут"), QIcon(),
                DBSTRUCTGUIDXML::ATTR, QString());


    // Создание описания состава
    query.exec("CREATE TABLE \"CLS" + DBCOMPXML::COMP + "\" ("
               "\"" + DBCOMPXML::LINKCLASS          + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBCLASSXML::CLASS + "\","
               "\"" + DBCOMPXML::ALIAS              + "\" VARCHAR(256),"
               "\"" + DBCOMPXML::PARENT             + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBCLASSXML::CLASS + "\","
               "\"" + DBCOMPXML::CLASS              + "\" CHAR(36),"
               "\"" + DBCOMPXML::ISVIEW             + "\" BOOL,"
               "\"" + DBCOMPXML::DIRECTDESCRIPTION  + "\" VARCHAR(256),"
               "\"" + DBCOMPXML::INVERSEDESCRIPTION + "\" VARCHAR(256),"
               "\"" + DBCOMPXML::ISCOMP             + "\" BOOL,"
               "\"" + DBCOMPXML::ID                 + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBCOMPXML::COMP, QObject::tr("Состав"), QIcon(),
                DBSTRUCTGUIDXML::COMP, QString());

    // Создание описания фильтра
    query.exec("CREATE TABLE \"CLS" + DBFILTERXML::FILTER + "\" ("
               "\"" + DBFILTERXML::NAME               + "\" VARCHAR(27),"
               "\"" + DBFILTERXML::ALIAS              + "\" VARCHAR(256),"
               "\"" + DBFILTERXML::PARENT             + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBCLASSXML::CLASS + "\","
               "\"" + DBFILTERXML::CLASS              + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBCLASSXML::CLASS + "\","
               "\"" + DBFILTERXML::DIRECTDESCRIPTION  + "\" VARCHAR(256),"
               "\"" + DBFILTERXML::INVERSEDESCRIPTION + "\" VARCHAR(256),"
               "\"" + DBFILTERXML::ID                 + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBFILTERXML::FILTER, QObject::tr("Фильтр"), QIcon(),
                DBSTRUCTGUIDXML::FILTER, QString());


    // Создание описания блока условий фильтра
    query.exec("CREATE TABLE \"CLS" + DBFILTERBLOCKXML::BLOCK + "\" ("
               "\"" + DBFILTERBLOCKXML::PARENT + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBFILTERXML::FILTER + "\","
//               "\"" + DBFILTERBLOCKXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBFILTERBLOCKXML::LINKOF + "\" VARCHAR(10),"
               "\"" + DBFILTERBLOCKXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBFILTERBLOCKXML::BLOCK,
                QObject::tr("Блок условий фильтра"), QIcon(),
                DBSTRUCTGUIDXML::BLOCK, QString());


    // Создание описания условия фильтра
    query.exec("CREATE TABLE \"CLS" + DBCONDITIONXML::COND + "\" ("
               "\"" + DBCONDITIONXML::FIRSTATTR  + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBATTRXML::ATTR + "\","
//               "\"" + DBCONDITIONXML::ALIAS      + "\" VARCHAR(256),"
               "\"" + DBCONDITIONXML::PARENT     + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBFILTERXML::FILTER + "\""
                    " REFERENCES \"CLS" + DBFILTERBLOCKXML::BLOCK + "\","
               "\"" + DBCONDITIONXML::SECONDATTR + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBATTRXML::ATTR + "\","
               "\"" + DBCONDITIONXML::OPERATOR   + "\" VARCHAR(10),"
               "\"" + DBCONDITIONXML::LINKOF     + "\" VARCHAR(10),"
               "\"" + DBCONDITIONXML::ID         + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBCONDITIONXML::COND,
                QObject::tr("Условие фильтра"), QIcon(),
                DBSTRUCTGUIDXML::COND, QString());


    // Создание описания группы справочников
    query.exec("CREATE TABLE \"CLS" + DBREFGROUPXML::REFGROUP + "\" ("
               "\"" + DBREFGROUPXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBREFGROUPXML::NAME   + "\" VARCHAR(27),"
               "\"" + DBREFGROUPXML::PARENT + "\" CHAR(36),"
               "\"" + DBREFGROUPXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBREFGROUPXML::REFGROUP,
                QObject::tr("Группа справочников"), QIcon(),
                DBSTRUCTGUIDXML::REFGROUP, QString());


    // Создание описания справочника
    query.exec("CREATE TABLE \"CLS" + DBREFXML::REF + "\" ("
               "\"" + DBREFXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBREFXML::NAME   + "\" VARCHAR(27),"
               "\"" + DBREFXML::PARENT + "\" CHAR(36)"
                    " REFERENCES \"CLS"   + DBREFGROUPXML::REFGROUP + "\","
               "\"" + DBREFXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBREFXML::REF,
                QObject::tr("Справочник"), QIcon(),
                DBSTRUCTGUIDXML::REF, QString());


    // Создание описания фильтра справочника
    query.exec("CREATE TABLE \"CLS" + DBLINKTOFILTERXML::LINKTOFILTER + "\" ("
               "\"" + DBLINKTOFILTERXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBLINKTOFILTERXML::REFFILTER   + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBFILTERXML::FILTER + "\","
               "\"" + DBLINKTOFILTERXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBLINKTOFILTERXML::LINKTOFILTER,
                QObject::tr("Фильтр справочника"), QIcon(),
                DBSTRUCTGUIDXML::LINKTOFILTER, QString());


    // Создание описания ссылки на справочник
    query.exec("CREATE TABLE \"CLS" + DBLINKTOREFXML::LINKTOREF + "\" ("
               "\"" + DBLINKTOREFXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBLINKTOREFXML::REFREF   + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBREFXML::REF + "\","
               "\"" + DBLINKTOREFXML::PARENT + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBLINKTOFILTERXML::LINKTOFILTER + "\""
                    " REFERENCES \"CLS" + DBREFXML::REF + "\","
               "\"" + DBLINKTOREFXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBLINKTOREFXML::LINKTOREF,
                QObject::tr("Ссылка на справочник"), QIcon(),
                DBSTRUCTGUIDXML::LINKTOREF, QString());


    // Создание описания класса справочника
    query.exec("CREATE TABLE \"CLS" + DBLINKTOCLASSXML::LINKTOCLASS + "\" ("
               "\"" + DBLINKTOCLASSXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBLINKTOCLASSXML::REFCLASS   + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBCLASSXML::CLASS + "\","
               "\"" + DBLINKTOCLASSXML::PARENT + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBLINKTOFILTERXML::LINKTOFILTER + "\""
                    " REFERENCES \"CLS" + DBREFXML::REF + "\","
               "\"" + DBLINKTOCLASSXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBLINKTOCLASSXML::LINKTOCLASS,
                QObject::tr("Класс справочника"), QIcon(),
                DBSTRUCTGUIDXML::LINKTOCLASS, QString());

    query.exec("ALTER TABLE \"CLS" + DBLINKTOFILTERXML::LINKTOFILTER + "\" ADD COLUMN "
               "\"" + DBLINKTOFILTERXML::PARENT + "\" CHAR(36)"
               " REFERENCES \"CLS" + DBLINKTOCLASSXML::LINKTOCLASS + "\""
               " REFERENCES \"CLS" + DBLINKTOREFXML::LINKTOREF + "\""
               ";");


    // Создание описания роли
    query.exec("CREATE TABLE \"CLS" + DBROLEXML::ROLE + "\" ("
               "\"" + DBROLEXML::ALIAS       + "\" VARCHAR(256),"
               "\"" + DBROLEXML::NAME        + "\" VARCHAR(27),"
               "\"" + DBROLEXML::DESCRIPTION + "\" VARCHAR(4000),"
               "\"" + DBROLEXML::PARENT      + "\" CHAR(36),"
               "\"" + DBROLEXML::ID          + "\" CHAR(36) PRIMARY KEY"
               ");");

    insertClass(query, DBROLEXML::ROLE,
                QObject::tr("Роль"), QIcon(),
                DBSTRUCTGUIDXML::ROLE, QString());

    // Создание описания прав доступа
    query.exec("CREATE TABLE \"CLS" + DBPERMISSIONXML::PERMISSION + "\" ("
               "\"" + DBPERMISSIONXML::ROLE     + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBROLEXML::ROLE   + "\","
               "\"" + DBPERMISSIONXML::ISCREATE + "\" BOOL,"
               "\"" + DBPERMISSIONXML::ISREAD   + "\" BOOL,"
               "\"" + DBPERMISSIONXML::ISWRITE  + "\" BOOL,"
               "\"" + DBPERMISSIONXML::ISDELETE + "\" BOOL,"
               "\"" + DBPERMISSIONXML::ISBLOCK  + "\" BOOL,"
               "\"" + DBPERMISSIONXML::PARENT   + "\" CHAR(36)"
                    " REFERENCES \"CLS" + DBCLASSXML::CLASS + "\""
                    " REFERENCES \"CLS" + DBATTRXML::ATTR   + "\","
               "\"" + DBPERMISSIONXML::ID       + "\" CHAR(36) PRIMARY KEY"
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

    return NULL;
}

IDatabaseClass *PgDatabaseModel::oneClass(
        const QString &name, const QString &base)
{
    Q_UNUSED(name)
    Q_UNUSED(base)

    return NULL;
}

IDatabaseClasses *PgDatabaseModel::classList()
{
    return NULL;
}

IDatabaseClasses *PgDatabaseModel::classList(const QString &base)
{
    Q_UNUSED(base)

    return NULL;
}

}}
