#ifndef PLUGINMODEL_H
#define PLUGINMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "iplugin.h"
#include "pluginmodelitem.h"

class PluginModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit PluginModel(QObject *parent = 0);
    ~PluginModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    bool insertColumns(int column, int count, const QModelIndex &parent);
    bool removeColumns(int column, int count, const QModelIndex &parent);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &index) const;

    void setPlugins(const QHash<QString, IPlugin *> &plugins);
    void update();
signals:
public slots:
    void currentItemChanged(const QModelIndex index);

private:
    enum Column {Name, Read, Version, Vendor};
    PluginModelItem *itemFromIndex(const QModelIndex &index) const;
    PluginModelItem *items() const;

private:   
    int m_columnCount;
    PluginModelItem *m_items;
};

#endif // PLUGINMODEL_H
