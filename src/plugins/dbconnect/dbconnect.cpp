#include "dbconnect.h"

DbConnect::DbConnect(QObject *parent):
    QObject(parent), IPlugin("IMainWindow")
{
    
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(dbconnect, DbConnect)
#endif



