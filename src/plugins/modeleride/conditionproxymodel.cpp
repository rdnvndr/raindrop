#include "conditionproxymodel.h"
#include <treexmlmodel/treexmlmodel.h>
#include "dbxmlstruct.h"

struct PrivateModelIndex
{
    int r, c;
    void *p;
    const QAbstractItemModel *m;
};


ConditionProxyModel::ConditionProxyModel(QObject *parent) :
    QAbstractProxyModel(parent)
{
}
int ConditionProxyModel::columnCount(const QModelIndex& parent) const
{
    if(!sourceModel())
        return 0;

    return 4;
}

int ConditionProxyModel::rowCount(const QModelIndex& parent) const
{
    if(!sourceModel())
        return 0;

    return sourceModel()->rowCount(mapToSource(parent)) ;
}

QModelIndex ConditionProxyModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return mapFromSource(sourceModel()->index(row, column, mapToSource(parent))) ;
}

QModelIndex ConditionProxyModel::parent(const QModelIndex& index) const
{
    if (index.isValid() == false)
        return QModelIndex();

    return mapFromSource(sourceModel()->parent(mapToSource(index))) ;
}

QModelIndex ConditionProxyModel::mapFromSource(const QModelIndex& index) const
{
    if(index.isValid())
        return createIndex(index.row(), index.column(), index.internalPointer());

    return QModelIndex();
}

QModelIndex ConditionProxyModel::mapToSource(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    QModelIndex sourceIndex;
    PrivateModelIndex* hack = reinterpret_cast<PrivateModelIndex*>(&sourceIndex);

    hack->r = index.row();
    hack->c = index.column();
    hack->p = index.internalPointer();
    hack->m = sourceModel();

    if (!sourceIndex.isValid())
        return QModelIndex();

    return sourceIndex;
}

QVariant ConditionProxyModel::data(const QModelIndex &proxyIndex, int role) const
{

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        TreeXMLModel *xmlModel = qobject_cast<TreeXMLModel *>(sourceModel());
        QModelIndex sourceIndex = mapToSource(proxyIndex);

        if (xmlModel->data(sourceIndex,Qt::UserRole)
                == DBFILTERBLOCKXML::BLOCK) {

            switch (proxyIndex.column()) {
            case 0:
                return QString("(...)");
                break;
            case 3:
                if (proxyIndex.row() < rowCount(proxyIndex.parent())-1)
                    return xmlModel->data(
                                sourceIndex.sibling(
                                    sourceIndex.row(),
                                    xmlModel->columnDisplayedAttr(
                                        DBFILTERBLOCKXML::BLOCK,
                                        DBFILTERBLOCKXML::LINKOF)),
                                role);
            default:
                return QVariant();
            }
        }

        if (xmlModel->data(sourceIndex,Qt::UserRole)
                == DBCONDITIONXML::COND) {

            switch (proxyIndex.column()) {
            case 0:
                return xmlModel->data(
                            sourceIndex.sibling(
                                sourceIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::FIRSTATTR)),
                            role);
            case 1:
                return xmlModel->data(
                            sourceIndex.sibling(
                                sourceIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::OPERATOR)),
                            role);
            case 2:
                return xmlModel->data(
                            sourceIndex.sibling(
                                sourceIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::SECONDATTR)),
                            role);
            case 3:
                if (proxyIndex.row() < rowCount(proxyIndex.parent())-1)
                    return xmlModel->data(
                                sourceIndex.sibling(
                                    sourceIndex.row(),
                                    xmlModel->columnDisplayedAttr(
                                        DBCONDITIONXML::COND,
                                        DBCONDITIONXML::LINKOF)),
                                role);
            default:
                return QVariant();
            }
        }
    }
    return QVariant();
}

bool ConditionProxyModel::setData(const QModelIndex &proxyIndex, const QVariant &value, int role)
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        TreeXMLModel *xmlModel = qobject_cast<TreeXMLModel *>(sourceModel());
        QModelIndex sourceIndex = mapToSource(proxyIndex);

        if (xmlModel->data(sourceIndex,Qt::UserRole)
                == DBFILTERBLOCKXML::BLOCK) {

            int column;
            switch (proxyIndex.column()) {
            case 3:
                column = xmlModel->columnDisplayedAttr(
                            DBFILTERBLOCKXML::BLOCK,
                            DBFILTERBLOCKXML::LINKOF);
                if (proxyIndex.row() == rowCount(proxyIndex.parent())-1) {
                    if (proxyIndex.row()==0)
                        return xmlModel->setData(
                                    sourceIndex.sibling(sourceIndex.row(),column),
                                    "И",
                                    role);
                    else
                        return xmlModel->setData(
                                    sourceIndex.sibling(sourceIndex.row(), column),
                                    data(proxyIndex.sibling(proxyIndex.row()-1, 3),
                                         role),
                                    role);
                } else {
                    return xmlModel->setData(
                                sourceIndex.sibling(sourceIndex.row(),column),
                                value,
                                role);
                }
            default:
                return false;
            }
        }

        if (xmlModel->data(sourceIndex,Qt::UserRole)
                == DBCONDITIONXML::COND) {

            int column;
            switch (proxyIndex.column()) {
            case 0:
                return xmlModel->setData(
                            sourceIndex.sibling(
                                sourceIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::FIRSTATTR)),
                            value,
                            role);
                break;
            case 1:
                return xmlModel->setData(
                            sourceIndex.sibling(
                                sourceIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::OPERATOR)),
                            value,
                            role);
                break;
            case 2:
                return xmlModel->setData(
                            sourceIndex.sibling(
                                sourceIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::SECONDATTR)),
                            value,
                            role);
                break;
            case 3:
                column = xmlModel->columnDisplayedAttr(
                            DBCONDITIONXML::COND,
                            DBCONDITIONXML::LINKOF);

                if (proxyIndex.row() == rowCount(proxyIndex.parent())-1) {
                    if (proxyIndex.row()==0)
                        return xmlModel->setData(
                                    sourceIndex.sibling(sourceIndex.row(),column),
                                    "И",
                                    role);
                    else
                        return xmlModel->setData(
                                    sourceIndex.sibling(sourceIndex.row(),column),
                                    data(proxyIndex.sibling(proxyIndex.row()-1, 3), role),
                                    role);
                } else
                    return xmlModel->setData(
                                sourceIndex.sibling(sourceIndex.row(),column),
                                value,
                                role);
            default:
                return false;
            }

        }
    }

    return false;
}

bool ConditionProxyModel::hasChildren(const QModelIndex &parent) const
{
    return rowCount(parent);
}
