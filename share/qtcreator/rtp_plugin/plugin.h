#ifndef %PluginName:u%_%CppHeaderSuffix:u%
#define %PluginName:u%_%CppHeaderSuffix:u%

#include <QObject>
#include <plugin/iplugin.h>
#include "%PluginName:l%global.%CppHeaderSuffix:l%"

class  %PluginName:u%LIB %PluginName%:
        public QObject,
        public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "com.RTPTechGroup.Raindrop.%PluginName%" FILE "%PluginName:l%.json")
#endif

public:

    //! Конструктор плагина
    explicit %PluginName%(QObject *parent = 0);

    //! Инициализация плагина
    bool initialize();

    //! Освобождение плагина
    bool release();

    //! Получение экземпляра
    QObject *instance() { return this; }
};

#endif
