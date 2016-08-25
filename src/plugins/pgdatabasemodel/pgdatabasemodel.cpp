#include "pgdatabasemodel.h"

PgDatabaseModel::PgDatabaseModel(QObject *parent):
    QObject(parent), IPlugin("")
{

}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdatabasemodel, PgDatabaseModel)
#endif


