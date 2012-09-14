#include "iguiplugin.h"

IGuiPlugin::IGuiPlugin(QObject *parent) :
    IPlugin(parent)
{
}

IGuiPlugin::~IGuiPlugin()
{
}

QIcon IGuiPlugin::icon()
{
    return m_pluginIcon;
}

void IGuiPlugin::setIcon(QIcon icon)
{
    m_pluginIcon = icon;
}
