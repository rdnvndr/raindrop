#include "actiongroupmodel.h"
#include "mimedataobject.h"

#include <QDebug>

ActionGroupModel::ActionGroupModel(QMultiHash<QString, QAction *> *actions)
{
    setActionGroup(actions);
}

QVariant ActionGroupModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.internalId()>0) {
        QString key = m_actions->uniqueKeys().at(index.internalId()-1);
        QAction *action = m_actions->values(key).at(index.row());
        if (role == Qt::DecorationRole) {
            if (action->icon().isNull())
                return QIcon(":empty");
            else
                return action->icon();
        } else if (role == Qt::DisplayRole || role == Qt::EditRole){
            return action->text();
        }
    } else if (role == Qt::DisplayRole || role == Qt::EditRole){
        QString key = m_actions->uniqueKeys().at(index.row());
        return key;
    }

    return QVariant();
}

Qt::ItemFlags ActionGroupModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled |
            Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ActionGroupModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        if (section == 0)
            return  tr("Наименование");
    }
    return  QVariant();
}

QModelIndex ActionGroupModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (parent.isValid()) {
        return createIndex(row, 0, parent.row()+1);
    } else
        return createIndex(row, 0);
}


QModelIndex ActionGroupModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();


    if (child.internalId()>0) {
        return createIndex(child.internalId()-1,0);
    } else
        return QModelIndex();
}

int ActionGroupModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_actions->uniqueKeys().count();
    } else {
        QString key = m_actions->uniqueKeys().at(parent.row());
        return m_actions->values(key).count();
    }

}

int ActionGroupModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QStringList ActionGroupModel::mimeTypes() const
{
    QStringList types;
    types << "application/x-qobject";
    return types;
}

QMimeData *ActionGroupModel::mimeData(const QModelIndexList &indexes) const
{
    foreach (const QModelIndex &index, indexes)
        if (index.internalId() > 0) {
            QString key = m_actions->uniqueKeys().at(index.internalId()-1);
            QAction *action = m_actions->values(key).at(index.row());          
            MimeDataObject *mimeData = new MimeDataObject();
            mimeData->setObject(action);
            return mimeData;
        }
    return 0;
}

void ActionGroupModel::setActionGroup(QMultiHash<QString, QAction *> *actions)
{
    m_actions = actions;
}

Qt::DropActions ActionGroupModel::supportedDragActions() const
{
    return Qt::CopyAction;
}

Qt::DropActions ActionGroupModel::supportedDropActions() const
{
    return Qt::MoveAction;
}
