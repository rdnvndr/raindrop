#include "%PluginName:l%.%CppHeaderSuffix:l%"

%PluginName%::%PluginName%(QObject *parent):
    QObject(parent)
{
    setName(tr("%PluginName%"));
    setDescript(tr(""));
    setVendor(tr("RTPTechGroup"));
    setVersion("0.0.1");
    setIcon(QIcon(":/%PluginName:l%"));
}

bool %PluginName%::initialize()
{
    return true;
}

bool %PluginName%::release()
{
    return true;
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(%PluginName:l%, %PluginName%)
#endif


