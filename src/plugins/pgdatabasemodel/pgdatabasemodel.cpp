#include "pgdatabasemodel.h"

#include "pgdatabaseattr.h"

PgDatabaseModel::PgDatabaseModel(QObject *parent):
    QObject(parent), IPlugin("")
{
    IDatabaseAttr *m_attr = new PgDatabaseAttr();
    QVariant x(10);
    IDatabaseExpression *e1 = NULL;
    e1 = &(*m_attr == *m_attr);
    IDatabaseExpression *e2 = NULL;
    e2 = &(*m_attr == 10.5);
    IDatabaseExpression *e3 = NULL;
    e3 = &(*e1 && *e2);
    e3 = &((*m_attr == *m_attr) && (*m_attr == 10.5));
    e3 = &(*e1 && *e2);
}

PgDatabaseModel::~PgDatabaseModel()
{

}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdatabasemodel, PgDatabaseModel)
#endif


