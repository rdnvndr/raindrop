#include "pluginmodel.h"
#include "common.h"

PluginModel::PluginModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_columnCount = 4;
    m_items = new PluginModelItem();
}

PluginModel::~PluginModel()
{
    DELETE_OBJECT(m_items);
}

QVariant PluginModel::data(const QModelIndex &index, int role) const
{
    if(!items()->isValid() || !index.isValid() || index.column() < 0 ||index.column() >= m_columnCount)
        return QVariant();

    if(PluginModelItem *item = itemFromIndex(index))
    {
       if(role == Qt::DisplayRole || role == Qt::EditRole)
       {
            switch(index.column())
            {
                case Name:
                    {
                        if(item->type == PluginModelItem::Group)
                            return QVariant(item->name());
                        else if(item->type == PluginModelItem::Plugin)
                            return QVariant(item->plugin()->name());
                    }
                case Read:
                    {
                        if(item->type == PluginModelItem::Group)
                            return QVariant();
                        else if(item->type == PluginModelItem::Plugin)
                            return QVariant(true);
                    }
                case Version:
                    {
                        if(item->type == PluginModelItem::Group)
                            return QVariant();
                        else if(item->type == PluginModelItem::Plugin)
                            return QVariant(item->plugin()->version());
                    }
                case Vendor:
                    {
                        if(item->type == PluginModelItem::Group)
                            return QVariant();
                        else if(item->type == PluginModelItem::Plugin)
                            return QVariant(item->plugin()->vendor());
                    }
                default:Q_ASSERT(false);
            }
       }

       if(role == Qt::DecorationRole)
       {
           if(index.column() == 0)
           {
               if(item->type == PluginModelItem::Group)
                   return QVariant();
               else if(item->type == PluginModelItem::Plugin)
               {
                    if(item->plugin()->state == IPlugin::Init)
                        return QVariant(QIcon(":/plugin"));
                    else if(item->plugin()->state == IPlugin::Error)
                        return QVariant(QIcon(":/plugin_error"));
               }
           }
       }
    }

    if(role == Qt::TextAlignmentRole)
    {
        if(index.column() == Name)
            return static_cast<int>(Qt::AlignVCenter|Qt::AlignLeft);
        else
            return static_cast<int>(Qt::AlignVCenter|Qt::AlignLeft);
    }
    return QVariant();
}

bool PluginModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);

    return true;
}

Qt::ItemFlags PluginModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if(index.isValid())
    {
        flags |= Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        if(index.column() == 0)
        {
            flags |= Qt::ItemIsDragEnabled |Qt::ItemIsDropEnabled;
        }
    }
    return flags;
}

QVariant PluginModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation ==Qt::Horizontal && role ==Qt::DisplayRole )
    {
        switch(section)
        {
            case Name:
                {
                    return QVariant(tr("Наименование"));
                }
            case Read:
                {
                    return QVariant(tr("Загружать"));
                }
            case Version:
                {
                    return QVariant(tr("Версия"));
                }
            case Vendor:
                {
                    return QVariant(tr("Поставщик"));
                }
            default:Q_ASSERT(false);
        }
    }
    return QVariant();
}

bool PluginModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(value);
    Q_UNUSED(role);

    return false;
}

int PluginModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid() && parent.column() != 0)
    {
        return 0;
    }

    PluginModelItem *parenNode = itemFromIndex(parent);
    return parenNode ? parenNode->count() : 0;
}

int PluginModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() && parent.column() != 0 ? 0 : m_columnCount;
}

bool PluginModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    return false;
}

bool PluginModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
    Q_UNUSED(parent);
    return false;
}

bool PluginModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(column);
    Q_UNUSED(count);
    return false;
}

bool PluginModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(column);
    Q_UNUSED(count);
    return false;
}

QModelIndex PluginModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!items()->isValid() || row < 0 || column < 0 || column >= m_columnCount
            || (parent.isValid() && parent.column() != 0))
        return QModelIndex();

    PluginModelItem *parentNode = itemFromIndex(parent);
    Q_ASSERT(parentNode);

    if(PluginModelItem *node = parentNode->itemAt(row))
        return createIndex(row, column, node);

    return QModelIndex();
}

QModelIndex PluginModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
        return QModelIndex();

    if(PluginModelItem *item = itemFromIndex(index))
    {
        if(PluginModelItem *parentItem = item->parent())
        {
            if(parentItem == items())
                return QModelIndex();

            else if(PluginModelItem *grandParentItem = parentItem->parent())
            {
                int row = grandParentItem->indexItemOf(parentItem);
                return createIndex(row, 0, parentItem);
            }
        }
    }
    return QModelIndex();
}

/*!
 * \brief Установка инфомации о плагинах
 * \param plugins
 */
void PluginModel::setPlugins(const QHash<QString, IPlugin *> &plugins)
{
    QMap<QString, PluginModelItem*> category;
    QHashIterator<QString, IPlugin*> i(plugins);
    while (i.hasNext())
    {
        i.next();
        IPlugin *plugin = i.value();
        PluginModelItem* categoryItem(0);
        QMap<QString, PluginModelItem*>::const_iterator resultsCategory =  category.find(plugin->category());
        if (resultsCategory != category.end())
        {
            categoryItem = resultsCategory.value();
        }
        else
        {
            categoryItem = new PluginModelItem();
            categoryItem->type = PluginModelItem::Group;
            categoryItem->setName(plugin->category());
            category.insert(plugin->category(), categoryItem);
            m_items->append(categoryItem);
        }

        if(categoryItem != 0)
        {
            PluginModelItem* pluginItem = new PluginModelItem();
            pluginItem->type = PluginModelItem::Plugin;
            pluginItem->setPlugin(plugin);
            categoryItem->append(pluginItem);
        }
    }
}

/*!
 * \brief Получение к из индекса
 * \param index
 * \return
 */
PluginModelItem *PluginModel::itemFromIndex(const QModelIndex &index) const
{
    if(index.isValid())
    {
        if(PluginModelItem *item = static_cast<PluginModelItem*>(index.internalPointer()))
            return item;
    }
    return items();
}

PluginModelItem *PluginModel::items() const
{
    return m_items;
}

/*!
 * \brief Обновление данных о конфигурации
 */
void PluginModel::update()
{      
    reset();
    currentItemChanged(index(0, 0, QModelIndex()));
}

/*!
 * \brief Изменение текущего узла
 * \param index
 */
void PluginModel::currentItemChanged(const QModelIndex index)
{
    PluginModelItem *item = itemFromIndex(index);
    Q_UNUSED(item);
}

