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
    Q_PLUGIN_METADATA(IID "com.%VendorName%.Raindrop.%PluginName%" FILE "%PluginName:l%.json")
#endif

public:

    //! Конструктор плагина
    explicit %PluginName%(QObject *parent = 0);

// IPlugin

    //! Получение экземпляра
    QObject *instance() { return this; }

    //! Получение имени плагина
    QString name() {return tr("%Name%");};

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/%PluginName:l%");}

    //! Описание плагина
    QString descript() {return tr("%Description%");};

    //! Категория в которой состоит плагин
    QString category() {return tr("%Category%");};

    //! Версия плагина
    QString version() {return tr("%Version%");};

    //! Производитель плагина
    QString vendor() {return tr("%VendorName%");};
};

#endif
