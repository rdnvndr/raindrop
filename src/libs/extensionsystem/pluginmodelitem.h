#ifndef PLUGINMODELITEM_H
#define PLUGINMODELITEM_H

#include <QString>
#include <QList>
#include <QPointer>
#include "iplugin.h"

class PluginModelItem
{
public:
    PluginModelItem();
    ~PluginModelItem();

    enum TypeItem {Group, Plugin};
    TypeItem type;

    QString name() const;
    void setName(const QString &name);

    bool isValid();

    virtual PluginModelItem* parent() const;
    void setParent(PluginModelItem* parent);

    int count() const;
    virtual PluginModelItem* itemAt(int index) const;
    int indexItemOf(PluginModelItem* item) const;
    void append(PluginModelItem *item);
    void remove(int index);

    QList<PluginModelItem*> items() const;

    IPlugin* plugin() const;
    void setPlugin(IPlugin *plugin);

private:
    QString m_name;
    IPlugin* m_plugin;

    PluginModelItem* m_parent;
    QList<PluginModelItem*> m_items;
};

#endif // PLUGINMODELITEM_H
