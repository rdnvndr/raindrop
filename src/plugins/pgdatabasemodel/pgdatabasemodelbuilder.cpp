#include "pgdatabasemodelbuilder.h"

#include "pgdatabasemodel.h"

#include "pgdatabaseclasses.h"
#include "pgdatabasecomposition.h"
#include "pgdatabaseattribute.h"

namespace RTPTechGroup {
namespace DatabaseModel {

PgDatabaseModelBuilder::PgDatabaseModelBuilder(QObject *parent):
    QObject(parent), IPlugin("")
{
    /*
    IDatabaseAttribute *m_attr  = new PgDatabaseAttribute();
    IDatabaseComposition *m_comp  = new PgDatabaseComposition();
    QVariant x(10);
    IDatabaseExpression e1 = (*m_attr == *m_attr);
    IDatabaseExpression e2 = (*m_attr == 10.5);
    e1 = (*m_comp == *m_attr);
    e1 = (*m_comp == *m_comp);
    e2 = (*m_attr == *m_comp);
    IDatabaseExpression *e3 = new IDatabaseExpression();
    *e3 = (e1 && e2);
    *e3 = ((*m_attr == *m_attr) && (*m_attr == 10.5));
    *e3 = (e1 && e2);
    delete m_attr;
    delete m_comp;
    delete e3;
    */
}

PgDatabaseModelBuilder::~PgDatabaseModelBuilder()
{

}

IDatabaseModel *PgDatabaseModelBuilder::createModel(IDatabasePool *pool)
{
    return new PgDatabaseModel(pool);
}

QString PgDatabaseModelBuilder::implDriverName() const
{
    return "QPSQL";
}

}}
