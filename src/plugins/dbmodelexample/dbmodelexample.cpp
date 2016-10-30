#include "dbmodelexample.h"

DbModelExample::DbModelExample(QObject *parent):
    QObject(parent), IPlugin("IDatabaseModel")
{

}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(dbmodelexample, DbModelExample)
#endif


