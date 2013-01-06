#include "iplugin.h"

IPlugin::IPlugin()
{
    state = IPlugin::NoInit;
}

IPlugin::~IPlugin()
{
    foreach (IPlugin* plug,PluginManager::instance()->dependentPlugins(this))
        if (plug)
            delete plug;

    if (state.testFlag(IPlugin::Init)){
        if (m_settings)
            writeSettings();
        this->release();
    }
    qDebug() << "Delete plugin:" << name();
}

void IPlugin::setSettings(QSettings *s){
    m_settings = s;
}

void IPlugin::addDepend(QString s){
    depModulList.append(s);
}

void IPlugin::setName(QString name){
    m_PlugName = name;
}

void IPlugin::setIcon(QIcon icon){
    m_PlugIcon = icon;
}

QString IPlugin::descript() const
{
    return m_descript;
}

QSettings* IPlugin::settings()
{
    return m_settings;
}

void IPlugin::readSettings()
{
}

void IPlugin::writeSettings()
{
}

QString IPlugin::category() const
{
    return m_category;
}

QString IPlugin::version() const
{
    return m_version;
}

QString IPlugin::vendor() const
{
    return m_vendor;
}

void IPlugin::setDescript(QString descript)
{
    m_descript = descript;
}

void IPlugin::setCategory(const QString category)
{
    m_category = category;
}

void IPlugin::setVersion(const QString version)
{
    m_version = version;
}

void IPlugin::setVendor(const QString vendor)
{
    m_vendor = vendor;
}

QString IPlugin::name(){
    return m_PlugName;
}

QIcon IPlugin::icon(){
    return m_PlugIcon;
}

bool IPlugin::release()
{
    return true;
}
