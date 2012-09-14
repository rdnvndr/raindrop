#include "pluginmodelitem.h"

PluginModelItem::PluginModelItem()
{
}

PluginModelItem::~PluginModelItem()
{
    qDeleteAll(m_items);
}

QString PluginModelItem::name() const
{
    return m_name;
}

void PluginModelItem::setName(const QString &name)
{
    m_name = name;
}

bool PluginModelItem::isValid()
{
    return count() != 0;
}

PluginModelItem *PluginModelItem::parent() const
{
    return m_parent;
}

void PluginModelItem::setParent(PluginModelItem *parent)
{
    m_parent = parent;
}

int PluginModelItem::count() const
{
    return m_items.count();
}

PluginModelItem *PluginModelItem::itemAt(int index) const
{
    return m_items.value(index, 0);
}

int PluginModelItem::indexItemOf(PluginModelItem *item) const
{   
    return  m_items.indexOf(item);
}

void PluginModelItem::append(PluginModelItem *item)
{
    item->setParent(this);
    m_items.append(item);
}

void PluginModelItem::remove(int index)
{
    m_items.removeAt(index);
}

QList<PluginModelItem *> PluginModelItem::items() const
{
    return m_items;
}

IPlugin * PluginModelItem::plugin() const
{
    return m_plugin;
}

void PluginModelItem::setPlugin(IPlugin *plugin)
{
    m_plugin = plugin;
}
