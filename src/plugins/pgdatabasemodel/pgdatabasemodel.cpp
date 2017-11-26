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

    query.exec("CREATE TABLE \"" + DBCLASSXML::CLASS + "\" ("
               "\"" + DBCLASSXML::NAME     + "\" VARCHAR(27),"
               "\"" + DBCLASSXML::MODE     + "\" CHAR(36),"
               "\"" + DBCLASSXML::TYPE     + "\" CHAR(36),"
               "\"" + DBCLASSXML::ALIAS    + "\" VARCHAR(256),"
               "\"" + DBCLASSXML::PARENT   + "\" CHAR(36),"
               "\"" + DBCLASSXML::TEMPLATE + "\" VARCHAR(4000),"
               "\"" + DBCLASSXML::VERCOUNT + "\" INTEGER,"
               "\"" + DBCLASSXML::ICON     + "\" CHAR(36),"
               "\"" + DBCLASSXML::ID       + "\" CHAR(36)"
               ");");

    query.exec("CREATE TABLE \"" + DBATTRXML::ATTR + "\" ("
               "\"" + DBATTRXML::NAME           + "\" VARCHAR(27),"
               "\"" + DBATTRXML::ALIAS          + "\" VARCHAR(256),"
               "\"" + DBATTRXML::TYPE           + "\" VARCHAR(15),"
               "\"" + DBATTRXML::GROUP          + "\" VARCHAR(256),"
               "\"" + DBATTRXML::MAXSTRLEN      + "\" INTEGER,"
               "\"" + DBATTRXML::ACCURACY       + "\" INTEGER,"
               "\"" + DBATTRXML::ISNULLALLOWED  + "\" BOOL,"
               "\"" + DBATTRXML::ISUNIQUE       + "\" BOOL,"
               "\"" + DBATTRXML::ISCANDIDATEKEY + "\" BOOL,"
               "\"" + DBATTRXML::INITIALVAL     + "\" VARCHAR(4000),"
               "\"" + DBATTRXML::LOWERBOUND     + "\" VARCHAR(4000),"
               "\"" + DBATTRXML::UPPERBOUND     + "\" VARCHAR(4000),"
               "\"" + DBATTRXML::REFCLASS       + "\" CHAR(36),"
               "\"" + DBATTRXML::REFUNIT        + "\" CHAR(36),"
               "\"" + DBATTRXML::REFLOV         + "\" CHAR(36),"
               "\"" + DBATTRXML::REFNUMERATOR   + "\" CHAR(36),"
               "\"" + DBATTRXML::PARENT         + "\" CHAR(36),"
               "\"" + DBATTRXML::ID             + "\" CHAR(36)"
               ");");

    query.exec("CREATE TABLE \"" + DBCOMPXML::COMP + "\" ("
               "\"" + DBCOMPXML::LINKCLASS          + "\" CHAR(36),"
               "\"" + DBCOMPXML::ALIAS              + "\" VARCHAR(256),"
               "\"" + DBCOMPXML::PARENT             + "\" CHAR(36),"
               "\"" + DBCOMPXML::CLASS              + "\" CHAR(36),"
               "\"" + DBCOMPXML::ISVIEW             + "\" BOOL,"
               "\"" + DBCOMPXML::DIRECTDESCRIPTION  + "\" VARCHAR(256),"
               "\"" + DBCOMPXML::INVERSEDESCRIPTION + "\" VARCHAR(256),"
               "\"" + DBCOMPXML::ISCOMP             + "\" BOOL,"
               "\"" + DBCOMPXML::ID                 + "\" CHAR(36)"
               ");");

    query.exec("CREATE TABLE \"" + DBFILTERXML::FILTER + "\" ("
               "\"" + DBFILTERXML::NAME               + "\" VARCHAR(27),"
               "\"" + DBFILTERXML::ALIAS              + "\" VARCHAR(256),"
               "\"" + DBFILTERXML::PARENT             + "\" CHAR(36),"
               "\"" + DBFILTERXML::CLASS              + "\" CHAR(36),"
               "\"" + DBFILTERXML::DIRECTDESCRIPTION  + "\" VARCHAR(256),"
               "\"" + DBFILTERXML::INVERSEDESCRIPTION + "\" VARCHAR(256),"
               "\"" + DBFILTERXML::ID                 + "\" CHAR(36)"
               ");");

    query.exec("CREATE TABLE \"" + DBCONDITIONXML::COND + "\" ("
               "\"" + DBCONDITIONXML::FIRSTATTR  + "\" CHAR(36),"
//               "\"" + DBCONDITIONXML::ALIAS      + "\" VARCHAR(256),"
               "\"" + DBCONDITIONXML::PARENT     + "\" CHAR(36),"
               "\"" + DBCONDITIONXML::SECONDATTR + "\" CHAR(36),"
               "\"" + DBCONDITIONXML::OPERATOR   + "\" VARCHAR(10),"
               "\"" + DBCONDITIONXML::LINKOF     + "\" VARCHAR(10),"
               "\"" + DBCONDITIONXML::ID         + "\" CHAR(36)"
               ");");

    query.exec("CREATE TABLE \"" + DBFILTERBLOCKXML::BLOCK + "\" ("
               "\"" + DBFILTERBLOCKXML::PARENT + "\" CHAR(36),"
//               "\"" + DBFILTERBLOCKXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBFILTERBLOCKXML::LINKOF + "\" VARCHAR(10),"
               "\"" + DBFILTERBLOCKXML::ID     + "\" CHAR(36)"
               ");");

    query.exec("CREATE TABLE \"" + DBQUANTITYXML::QUANTITY + "\" ("
               "\"" + DBQUANTITYXML::BASICUNIT + "\" CHAR(36),"
               "\"" + DBQUANTITYXML::ALIAS     + "\" VARCHAR(256),"
               "\"" + DBQUANTITYXML::NAME      + "\" VARCHAR(256),"
               "\"" + DBQUANTITYXML::DIMENSION + "\" VARCHAR(256),"
               "\"" + DBQUANTITYXML::PARENT    + "\" CHAR(36),"
               "\"" + DBQUANTITYXML::ID        + "\" CHAR(36)"
               ");");

    query.exec("CREATE TABLE \"" + DBUNITXML::UNIT + "\" ("
               "\"" + DBUNITXML::CODE           + "\" INTEGER,"
               "\"" + DBUNITXML::COEFF          + "\" DOUBLE PRECISION,"
               "\"" + DBUNITXML::DELTA          + "\" DOUBLE PRECISION,"
               "\"" + DBUNITXML::NAME           + "\" VARCHAR(256),"
               "\"" + DBUNITXML::ID             + "\" CHAR(36),"
               "\"" + DBUNITXML::DESIGNATION    + "\" VARCHAR(32),"
               "\"" + DBUNITXML::INTDESIGNATION + "\" VARCHAR(32),"
               "\"" + DBUNITXML::SYMBOL         + "\" VARCHAR(32),"
               "\"" + DBUNITXML::INTSYMBOL      + "\" VARCHAR(32),"
//               "\"" + DBUNITXML::ALIAS          + "\" VARCHAR(256),"
               "\"" + DBUNITXML::PARENT         + "\" CHAR(36)"
               ");");

    query.exec("CREATE TABLE \"" + DBQUANTITYGROUPXML::QUANTITYGROUP + "\" ("
               "\"" + DBQUANTITYGROUPXML::NAME   + "\" VARCHAR(27),"
               "\"" + DBQUANTITYGROUPXML::ALIAS  + "\" VARCHAR(256),"
               "\"" + DBQUANTITYGROUPXML::PARENT + "\" CHAR(36),"
               "\"" + DBQUANTITYGROUPXML::ID     + "\" CHAR(36)"
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
