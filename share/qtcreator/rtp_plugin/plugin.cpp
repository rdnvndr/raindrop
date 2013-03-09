#include "%PluginName:l%.%CppHeaderSuffix:l%"

%PluginName%::%PluginName%(QObject *parent):
    QObject(parent), IPlugin("")
{

}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(%PluginName:l%, %PluginName%)
#endif


