#include "iplugin.h"

IPlugin::IPlugin(QObject *parent):
    QObject(parent)
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
        {
            QString className = PluginManager::instance()->pluginClass(this);
            m_settings->beginGroup(className);
            writeSettings();
            m_settings->endGroup();
            m_settings->sync();
        }
        release();
    }
    qDebug() << "Delete plugin:" << PluginManager::instance()->pluginClass(this);
}

void IPlugin::publicObject(QObject *obj)
{
    obj->setObjectName(metaObject()->className()+QString("::")+obj->objectName());
    PluginManager::instance()->addObject(obj);
}

void IPlugin::privateObject(QObject *obj)
{
    QString objName = obj->objectName();
    objName = objName.left(objName.indexOf("::")+3);
    obj->setObjectName(objName);
    PluginManager::instance()->removeObject(obj);
}

void IPlugin::readSettings(){

}


void IPlugin::writeSettings(){

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

bool IPlugin::initialize()
{
    return true;
}
