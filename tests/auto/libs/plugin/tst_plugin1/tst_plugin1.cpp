#include "tst_plugin1.h"

tst_plugin1::tst_plugin1(QObject *parent):
    QObject(parent), IPlugin("Itst_plugin4")
{
    
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(tst_plugin1, tst_plugin1)
#endif



