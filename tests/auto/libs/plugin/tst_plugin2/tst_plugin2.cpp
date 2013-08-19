#include "tst_plugin2.h"

tst_plugin2::tst_plugin2(QObject *parent):
    QObject(parent), IPlugin("Itst_plugin3")
{
    
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(tst_plugin2, tst_plugin2)
#endif



