#include "extendedapplication.h"
#include <pluginmanager.h>
#include <imainwindow.h>
#include <QtGui>

ExtendedApplication::ExtendedApplication(int &argc, char **argv) :
    QApplication(argc, argv)
{
}

ExtendedApplication::~ExtendedApplication()
{
    PluginManager::release();   
}

/*!
 * \brief Предоставляет экземпляр приложения
 * \return
 */
ExtendedApplication *ExtendedApplication::instance()
{
    return (static_cast<ExtendedApplication *>(QCoreApplication::instance()));
}

// Settings
void ExtendedApplication::setSettings(QPointer<QSettings> settings)
{
    m_settings = settings;
}

QPointer<QSettings> ExtendedApplication::settings()
{
    if (m_settings.isNull())
    {
        m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                     QLatin1String("RTPTechGroup"), QLatin1String("UniversalPlatform"));
    }
    return m_settings;
}

/*!
 *  \brief Чтение настроек
 */
void ExtendedApplication::readSettings()
{
}

/*!
 * \brief Запись настроек
 */
void ExtendedApplication::writeSettings()
{
}

/*!
 * \brief Загрузка переводов приложения
 */
void ExtendedApplication::localize()
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    // Подключение перевода для Qt
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(this);
    QString translationsPath(QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QDir translationDir(translationsPath);
    if(!translationDir.exists())
    {
        translationDir = QDir(qApp->applicationDirPath());
        translationDir.cd("translations");
        translationsPath = translationDir.absolutePath();
    }

    if (translator->load(translatorFileName, translationsPath))
        installTranslator(translator);

    // Подключение переводов приложения
    // Переводы находятся в <Каталог приложения>\translations\<Системная локализация>
    translationDir = QDir(qApp->applicationDirPath());
    if (translationDir.cd("translations\\"+QLocale::system().name()))
        foreach(QString fileName, translationDir.entryList(QDir::Files))
            if (translator->load(fileName,translationDir.absolutePath()))
                installTranslator(translator);
}

/*!
 * \brief Инициализация приложения
 * \return
 */
bool ExtendedApplication::initialize()
{
    bool result(false);
    QSplashScreen *splash = new QSplashScreen();
    splash->setPixmap(QPixmap(":/splash"));

    PluginManager *pluginManager = PluginManager::instance();
    connect(pluginManager, SIGNAL(showMessage(QString, int, QColor)),
                splash, SLOT(showMessage(QString, int, QColor)));

    pluginManager->setSettings(settings());
    pluginManager->readSettings();

    if(pluginManager->loadPlugins())
    {
        IMainWindow *mainWindowPlugin = qobject_cast<IMainWindow*>(pluginManager->pluginByName("MainWindow"));
        QMainWindow* mainWindow = mainWindowPlugin->mainWindow();
        splash->finish(mainWindow);
        result = true;
    }

    delete splash;
    return result;
}

