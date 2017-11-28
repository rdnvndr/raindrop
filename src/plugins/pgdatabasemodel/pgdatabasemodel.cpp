#include "pgdatabasemodel.h"

#include <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseModel::PgDatabaseModel(QSqlDatabase db)
{
    m_db = db;
}

bool PgDatabaseModel::init()
{
    m_db.transaction();
    QSqlQuery query = QSqlQuery(m_db);

    query.exec("CREATE TABLE \"" + DBLOVXML::LOV + "\" ("
               "\"" + DBLOVXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBLOVXML::NAME   + "\" VARCHAR(27),"
               "\"" + DBLOVXML::TYPE   + "\" VARCHAR(15),"
               "\"" + DBLOVXML::PARENT + "\" CHAR(36),"
               "\"" + DBLOVXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    // DBLOVVALUEXML::LOVVALUE - Таблицы

    query.exec("CREATE TABLE \"" + DBATTRTYPEXML::ATTRTYPE + "\" ("
               "\"" + DBLOVVALUEXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBLOVVALUEXML::VALUE  + "\" VARCHAR(256),"
               "\"" + DBLOVVALUEXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBCLASSTYPEXML::CLASSTYPE + "\" ("
               "\"" + DBLOVVALUEXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBLOVVALUEXML::VALUE  + "\" VARCHAR(256),"
               "\"" + DBLOVVALUEXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBACCESSMODEXML::ACCESSMODE + "\" ("
               "\"" + DBLOVVALUEXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBLOVVALUEXML::VALUE  + "\" VARCHAR(256),"
               "\"" + DBLOVVALUEXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBUNIQUENUMERATORXML::UNIQUENUMERATOR + "\" ("
               "\"" + DBLOVVALUEXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBLOVVALUEXML::VALUE  + "\" VARCHAR(256),"
               "\"" + DBLOVVALUEXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");



    query.exec("CREATE TABLE \"" + DBQUANTITYGROUPXML::QUANTITYGROUP + "\" ("
               "\"" + DBQUANTITYGROUPXML::NAME   + "\" VARCHAR(27),"
               "\"" + DBQUANTITYGROUPXML::ALIAS  + "\" VARCHAR(256),"
//               "\"" + DBQUANTITYGROUPXML::PARENT + "\" CHAR(36),"
               "\"" + DBQUANTITYGROUPXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBQUANTITYXML::QUANTITY + "\" ("
               "\"" + DBQUANTITYXML::ALIAS     + "\" VARCHAR(256),"
               "\"" + DBQUANTITYXML::NAME      + "\" VARCHAR(256),"
               "\"" + DBQUANTITYXML::DIMENSION + "\" VARCHAR(256),"
               "\"" + DBQUANTITYXML::PARENT    + "\" CHAR(36)"
                    " REFERENCES \"" + DBQUANTITYGROUPXML::QUANTITYGROUP + "\","
               "\"" + DBQUANTITYXML::ID        + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBUNITXML::UNIT + "\" ("
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
                    " REFERENCES \"" + DBQUANTITYXML::QUANTITY + "\""
               ");");

    query.exec("ALTER TABLE \"" + DBQUANTITYXML::QUANTITY + "\" ADD COLUMN "
               "\"" + DBQUANTITYXML::BASICUNIT + "\" CHAR(36)"
               " REFERENCES \"" + DBUNITXML::UNIT + "\""
               ";");


    query.exec("CREATE TABLE \"" + DBNUMERATORXML::NUMERATOR + "\" ("
               "\"" + DBNUMERATORXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBNUMERATORXML::NAME   + "\" VARCHAR(27),"
               "\"" + DBNUMERATORXML::STEP   + "\" INTEGER,"
               "\"" + DBNUMERATORXML::UNIQUE + "\" CHAR(36)"
                    " REFERENCES \"" + DBUNIQUENUMERATORXML::UNIQUENUMERATOR + "\","
               "\"" + DBNUMERATORXML::PARENT + "\" CHAR(36),"
               "\"" + DBNUMERATORXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBNUMERATORLOVXML::NUMERATORLOV + "\" ("
               "\"" + DBNUMERATORLOVXML::REFLOV + "\" CHAR(36)"
                    " REFERENCES \"" + DBLOVXML::LOV + "\","
               "\"" + DBNUMERATORLOVXML::PARENT + "\" CHAR(36)"
                    " REFERENCES \"" + DBNUMERATORXML::NUMERATOR + "\","
               "\"" + DBNUMERATORLOVXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBNUMERATORREGEXXML::NUMERATORREGEX + "\" ("
               "\"" + DBNUMERATORREGEXXML::REGEX  + "\" VARCHAR(256),"
               "\"" + DBNUMERATORREGEXXML::PARENT + "\" CHAR(36)"
                    " REFERENCES \"" + DBNUMERATORXML::NUMERATOR + "\","
               "\"" + DBNUMERATORREGEXXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");


    query.exec("CREATE TABLE \"" + DBCLASSXML::CLASS + "\" ("
               "\"" + DBCLASSXML::NAME     + "\" VARCHAR(27),"
               "\"" + DBCLASSXML::MODE     + "\" CHAR(36)"
                    " REFERENCES \"" + DBACCESSMODEXML::ACCESSMODE + "\","
               "\"" + DBCLASSXML::TYPE     + "\" CHAR(36)"
                    " REFERENCES \"" + DBCLASSTYPEXML::CLASSTYPE + "\","
               "\"" + DBCLASSXML::ALIAS    + "\" VARCHAR(256),"
               "\"" + DBCLASSXML::PARENT   + "\" CHAR(36)"
                    " REFERENCES \"" + DBCLASSXML::CLASS + "\","
               "\"" + DBCLASSXML::TEMPLATE + "\" VARCHAR(4000),"
               "\"" + DBCLASSXML::VERCOUNT + "\" INTEGER,"
               "\"" + DBCLASSXML::ICON     + "\" VARCHAR(4000),"
               "\"" + DBCLASSXML::ID       + "\" CHAR(36) PRIMARY KEY"
               ");");


    query.exec("CREATE TABLE \"" + DBATTRXML::ATTR + "\" ("
               "\"" + DBATTRXML::NAME           + "\" VARCHAR(27),"
               "\"" + DBATTRXML::ALIAS          + "\" VARCHAR(256),"
               "\"" + DBATTRXML::TYPE           + "\" CHAR(36)"
                    " REFERENCES \"" + DBATTRTYPEXML::ATTRTYPE + "\","
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
                    " REFERENCES \"" + DBCLASSXML::CLASS + "\","
               "\"" + DBATTRXML::REFUNIT        + "\" CHAR(36)"
                    " REFERENCES \"" + DBUNITXML::UNIT + "\","
               "\"" + DBATTRXML::REFLOV         + "\" CHAR(36)"
                    " REFERENCES \"" + DBLOVXML::LOV + "\","
               "\"" + DBATTRXML::REFNUMERATOR   + "\" CHAR(36)"
                    " REFERENCES \"" + DBNUMERATORXML::NUMERATOR + "\","
               "\"" + DBATTRXML::PARENT         + "\" CHAR(36)"
                    " REFERENCES \"" + DBCLASSXML::CLASS + "\","
               "\"" + DBATTRXML::ID             + "\" CHAR(36) PRIMARY KEY"
               ");");


    query.exec("CREATE TABLE \"" + DBCOMPXML::COMP + "\" ("
               "\"" + DBCOMPXML::LINKCLASS          + "\" CHAR(36)"
                    " REFERENCES \"" + DBCLASSXML::CLASS + "\","
               "\"" + DBCOMPXML::ALIAS              + "\" VARCHAR(256),"
               "\"" + DBCOMPXML::PARENT             + "\" CHAR(36)"
                    " REFERENCES \"" + DBCLASSXML::CLASS + "\","
               "\"" + DBCOMPXML::CLASS              + "\" CHAR(36),"
               "\"" + DBCOMPXML::ISVIEW             + "\" BOOL,"
               "\"" + DBCOMPXML::DIRECTDESCRIPTION  + "\" VARCHAR(256),"
               "\"" + DBCOMPXML::INVERSEDESCRIPTION + "\" VARCHAR(256),"
               "\"" + DBCOMPXML::ISCOMP             + "\" BOOL,"
               "\"" + DBCOMPXML::ID                 + "\" CHAR(36) PRIMARY KEY"
               ");");


    query.exec("CREATE TABLE \"" + DBFILTERXML::FILTER + "\" ("
               "\"" + DBFILTERXML::NAME               + "\" VARCHAR(27),"
               "\"" + DBFILTERXML::ALIAS              + "\" VARCHAR(256),"
               "\"" + DBFILTERXML::PARENT             + "\" CHAR(36)"
                    " REFERENCES \"" + DBCLASSXML::CLASS + "\","
               "\"" + DBFILTERXML::CLASS              + "\" CHAR(36)"
                    " REFERENCES \"" + DBCLASSXML::CLASS + "\","
               "\"" + DBFILTERXML::DIRECTDESCRIPTION  + "\" VARCHAR(256),"
               "\"" + DBFILTERXML::INVERSEDESCRIPTION + "\" VARCHAR(256),"
               "\"" + DBFILTERXML::ID                 + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBFILTERBLOCKXML::BLOCK + "\" ("
               "\"" + DBFILTERBLOCKXML::PARENT + "\" CHAR(36)"
                    " REFERENCES \"" + DBFILTERXML::FILTER + "\","
//               "\"" + DBFILTERBLOCKXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBFILTERBLOCKXML::LINKOF + "\" VARCHAR(10),"
               "\"" + DBFILTERBLOCKXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBCONDITIONXML::COND + "\" ("
               "\"" + DBCONDITIONXML::FIRSTATTR  + "\" CHAR(36)"
                    " REFERENCES \"" + DBATTRXML::ATTR + "\","
//               "\"" + DBCONDITIONXML::ALIAS      + "\" VARCHAR(256),"
               "\"" + DBCONDITIONXML::PARENT     + "\" CHAR(36)"
                    " REFERENCES \"" + DBFILTERXML::FILTER + "\""
                    " REFERENCES \"" + DBFILTERBLOCKXML::BLOCK + "\","
               "\"" + DBCONDITIONXML::SECONDATTR + "\" CHAR(36)"
                    " REFERENCES \"" + DBATTRXML::ATTR + "\","
               "\"" + DBCONDITIONXML::OPERATOR   + "\" VARCHAR(10),"
               "\"" + DBCONDITIONXML::LINKOF     + "\" VARCHAR(10),"
               "\"" + DBCONDITIONXML::ID         + "\" CHAR(36) PRIMARY KEY"
               ");");


    query.exec("CREATE TABLE \"" + DBREFGROUPXML::REFGROUP + "\" ("
               "\"" + DBREFGROUPXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBREFGROUPXML::NAME   + "\" VARCHAR(27),"
               "\"" + DBREFGROUPXML::PARENT + "\" CHAR(36),"
               "\"" + DBREFGROUPXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBREFXML::REF + "\" ("
               "\"" + DBREFXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBREFXML::NAME   + "\" VARCHAR(27),"
               "\"" + DBREFXML::PARENT + "\" CHAR(36)"
                    " REFERENCES \""   + DBREFGROUPXML::REFGROUP + "\","
               "\"" + DBREFXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBLINKTOFILTERXML::LINKTOFILTER + "\" ("
               "\"" + DBLINKTOFILTERXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBLINKTOFILTERXML::REFFILTER   + "\" CHAR(36)"
                    " REFERENCES \"" + DBFILTERXML::FILTER + "\","
               "\"" + DBLINKTOFILTERXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBLINKTOREFXML::LINKTOREF + "\" ("
               "\"" + DBLINKTOREFXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBLINKTOREFXML::REFREF   + "\" CHAR(36)"
                    " REFERENCES \"" + DBREFXML::REF + "\","
               "\"" + DBLINKTOREFXML::PARENT + "\" CHAR(36)"
                    " REFERENCES \"" + DBLINKTOFILTERXML::LINKTOFILTER + "\""
                    " REFERENCES \"" + DBREFXML::REF + "\","
               "\"" + DBLINKTOREFXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBLINKTOCLASSXML::LINKTOCLASS + "\" ("
               "\"" + DBLINKTOCLASSXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBLINKTOCLASSXML::REFCLASS   + "\" CHAR(36)"
                    " REFERENCES \"" + DBCLASSXML::CLASS + "\","
               "\"" + DBLINKTOCLASSXML::PARENT + "\" CHAR(36)"
                    " REFERENCES \"" + DBLINKTOFILTERXML::LINKTOFILTER + "\""
                    " REFERENCES \"" + DBREFXML::REF + "\","
               "\"" + DBLINKTOCLASSXML::ID     + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("ALTER TABLE \"" + DBLINKTOFILTERXML::LINKTOFILTER + "\" ADD COLUMN "
               "\"" + DBLINKTOFILTERXML::PARENT + "\" CHAR(36)"
               " REFERENCES \"" + DBLINKTOCLASSXML::LINKTOCLASS + "\""
               " REFERENCES \"" + DBLINKTOREFXML::LINKTOREF + "\""
               ";");


    query.exec("CREATE TABLE \"" + DBROLEXML::ROLE + "\" ("
               "\"" + DBROLEXML::ALIAS       + "\" VARCHAR(256),"
               "\"" + DBROLEXML::NAME        + "\" VARCHAR(27),"
               "\"" + DBROLEXML::DESCRIPTION + "\" VARCHAR(4000),"
               "\"" + DBROLEXML::PARENT      + "\" CHAR(36),"
               "\"" + DBROLEXML::ID          + "\" CHAR(36) PRIMARY KEY"
               ");");

    query.exec("CREATE TABLE \"" + DBPERMISSIONXML::PERMISSION + "\" ("
               "\"" + DBPERMISSIONXML::ROLE     + "\" CHAR(36)"
                    " REFERENCES \"" + DBROLEXML::ROLE   + "\","
               "\"" + DBPERMISSIONXML::ISCREATE + "\" BOOL,"
               "\"" + DBPERMISSIONXML::ISREAD   + "\" BOOL,"
               "\"" + DBPERMISSIONXML::ISWRITE  + "\" BOOL,"
               "\"" + DBPERMISSIONXML::ISDELETE + "\" BOOL,"
               "\"" + DBPERMISSIONXML::ISBLOCK  + "\" BOOL,"
               "\"" + DBPERMISSIONXML::PARENT   + "\" CHAR(36)"
                    " REFERENCES \"" + DBCLASSXML::CLASS + "\""
                    " REFERENCES \"" + DBATTRXML::ATTR   + "\","
               "\"" + DBPERMISSIONXML::ID       + "\" CHAR(36) PRIMARY KEY"
               ");");

    m_db.commit();
    return true;
}

IDatabaseClass *PgDatabaseModel::createDerivedClass(
        const QString &name, const QString &base)
{
    Q_UNUSED(name)
    Q_UNUSED(base)

    return NULL;
}

IDatabaseClass *PgDatabaseModel::derivedClass(
        const QString &name, const QString &base)
{
    Q_UNUSED(name)
    Q_UNUSED(base)

    return NULL;
}

IDatabaseClasses *PgDatabaseModel::allClassList()
{
    return NULL;
}

IDatabaseClasses *PgDatabaseModel::derivedClassList(const QString &base)
{
    Q_UNUSED(base)

    return NULL;
}

}}
