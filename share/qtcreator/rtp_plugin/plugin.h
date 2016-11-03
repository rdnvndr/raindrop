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
    Q_PLUGIN_METADATA(IID "com.%VendorName%.Raindrop.%PluginName%" FILE "%PluginName:l%.json")

public:

    //! Конструктор плагина
    explicit %PluginName%(QObject *parent = 0);

// IPlugin

    //! Получение имени плагина
    QString name() {return APP_NAME;}

    //! Получение иконки плагина
    QIcon icon() {return QIcon(":/%PluginName:l%");}

    //! Описание продукта
    QString product() {return APP_PRODUCT;}

    //! Авторские права
    QString copyright() {return APP_COPYRIGHT;}

    //! Описание плагина
    QString descript() {return tr(APP_DESCRIPTION);}

    //! Категория в которой состоит плагин
    QString category() {return tr("%Category%");}

    //! Версия плагина
    QString version() {return APP_VERSION;}

    //! Производитель плагина
    QString vendor() {return tr(APP_COMPANY);}
};

#endif
