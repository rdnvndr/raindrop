#include "actiongroupmodel.h"
#include "mimedataobject.h"

namespace RTPTechGroup {
namespace MainWindow {

ActionGroupModel::ActionGroupModel(QMultiHash<QString, QAction *> *actions)
{
    setActionGroup(actions);
}

QVariant ActionGroupModel::data(const QModelIndex &index, qint32 role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.internalId()>0) {

        QString key = m_actions->uniqueKeys().at(index.internalId()-1);
        QAction *action = m_actions->values(key).at(index.row());
        if (role == Qt::DecorationRole && index.column() == 0) {
            if (action->icon().isNull())
                return QIcon(":empty");
            else
                return action->icon();
        }

        if (role == Qt::DisplayRole || role == Qt::EditRole){
            switch (index.column()) {
                case 0: return action->text();
                case 1: return action->shortcut().toString();
                case 2: return action->whatsThis();
                case 3: return action->toolTip();
                case 4: return action->statusTip();
            }
        }

    } else if ((role == Qt::DisplayRole || role == Qt::EditRole)
               && index.column() == 0){
        QString key = m_actions->uniqueKeys().at(index.row());
        return key;
    }

    return QVariant();
}

bool ActionGroupModel::setData(const QModelIndex &index, const QVariant &value, qint32 role)
{
    if (role == Qt::EditRole && index.internalId()>0) {
        QString key = m_actions->uniqueKeys().at(index.internalId()-1);
        QAction *action = m_actions->values(key).at(index.row());
        switch (index.column()) {
        case 0:
            action->setText(value.toString());
            break;
        case 1:
            action->setShortcut(value.value<QKeySequence>());
            break;
        case 2:
            action->setWhatsThis(value.toString());
            break;
        case 3:
            action->setToolTip(value.toString());
            break;
        case 4:
            action->setStatusTip(value.toString());
            break;
        }
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

Qt::ItemFlags ActionGroupModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled |
            Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant ActionGroupModel::headerData(qint32 section, Qt::Orientation orientation, qint32 role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch (section) {
            case 0: return  tr("Наименование");
            case 1: return  tr("Сочетание клавиш");
            case 2: return  tr("Описание");
            case 3: return  tr("Подсказака");
            case 4: return  tr("Подсказка строки статуса");
        }
    }
    return  QVariant();
}

QModelIndex ActionGroupModel::index(qint32 row, qint32 column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (parent.isValid()) {
        return createIndex(row, column, parent.row()+1);
    } else
        return createIndex(row, column);
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

qint32 ActionGroupModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_actions->uniqueKeys().count();
    } else if (!parent.parent().isValid()) {
        QString key = m_actions->uniqueKeys().at(parent.row());
        return m_actions->values(key).count();
    } else
        return 0;

}

qint32 ActionGroupModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 5;
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

}
}
