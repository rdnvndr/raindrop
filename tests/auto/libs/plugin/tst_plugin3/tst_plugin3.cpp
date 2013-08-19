#include "tst_plugin3.h"

tst_plugin3::tst_plugin3(QObject *parent):
    QObject(parent), IPlugin("")
{
    
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(tst_plugin3, tst_plugin3)
#endif



