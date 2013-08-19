#include "tst_plugin4.h"

tst_plugin4::tst_plugin4(QObject *parent):
    QObject(parent), IPlugin("Itst_plugin3")
{
    
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(tst_plugin4, tst_plugin4)
#endif



