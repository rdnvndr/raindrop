#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLibraryInfo>
#include <plugin/pluginmanager.h>
#include "version.h"

int main(int argc, char* argv[])
{
    QApplication *app = new QApplication(argc,argv);
    app->setApplicationName(APP_NAME);
    #if QT_VERSION >= 0x050000
        app->setApplicationDisplayName(APP_PRODUCT);
    #endif
    app->setApplicationVersion(APP_VERSION);
    app->setOrganizationName(APP_COMPANY);
    app->setWindowIcon(QIcon(":raindrop"));

    app->setProperty("app_name",        APP_NAME);
    app->setProperty("app_product",     APP_PRODUCT);
    app->setProperty("app_version",     APP_VERSION);
    app->setProperty("app_company",     APP_COMPANY);
    app->setProperty("app_copyright",   APP_COPYRIGHT);
    app->setProperty("app_description", APP_DESCRIPTION);
    app->setProperty("app_revision",    VER_REV);

    #if QT_VERSION < 0x050000
        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    #endif

    // Подключение перевода для Qt
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(app);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app->installTranslator(translator);

    // Подключение переводов приложения
    // Переводы находятся в <Каталог приложения>\translations\<Системная локализация>
    QDir translationDir = QDir(qApp->applicationDirPath());
    if (translationDir.cd("translations\\"+QLocale::system().name()))
        foreach(QString fileName, translationDir.entryList(QDir::Files))
            if (translator->load(fileName,translationDir.absolutePath()))
                app->installTranslator(translator);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                                 QLatin1String("RTPTechGroup"), QLatin1String("Raindrop"));

    PluginManager pluginManager;
    pluginManager.setSettings(&settings);
    pluginManager.loadPlugins();

    return app->exec();
}
