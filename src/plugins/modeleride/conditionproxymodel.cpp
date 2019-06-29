#include "conditionproxymodel.h"
#include <treexmlmodel/treexmlhashmodel.h>
#include <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

ConditionProxyModel::ConditionProxyModel(QObject *parent) :
    ModifyProxyModel(parent)
{
}

qint32 ConditionProxyModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    if(!sourceModel())
        return 0;

    return 4;
}

QVariant ConditionProxyModel::data(const QModelIndex &proxyIndex, qint32 role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        TreeXmlHashModel *xmlModel = qobject_cast<TreeXmlHashModel *>(sourceModel());
        QString tagRole = data(proxyIndex,TreeXmlModel::TagRole).toString();

        if (tagRole == DBFILTERBLOCKXML::BLOCK) {
            switch (proxyIndex.column()) {
            case 0:
                return QString("{Блок}");
            case 3:
                if (proxyIndex.row() < rowCount(proxyIndex.parent())-1)
                    return ModifyProxyModel::data(
                                proxyIndex.sibling(
                                    proxyIndex.row(),
                                    xmlModel->columnDisplayedAttr(
                                        DBFILTERBLOCKXML::BLOCK,
                                        DBFILTERBLOCKXML::LINKOF)),
                                role);
                else
                    return QVariant();
            default:
                return QVariant();
            }
        }

        if (tagRole == DBCONDITIONXML::COND) {
            switch (proxyIndex.column()) {
            case 0: {
                QVariant value = ModifyProxyModel::data(
                            proxyIndex.sibling(
                                proxyIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::FIRSTATTR)),
                            role);
                return value;
            }
            case 1:
                return ModifyProxyModel::data(
                            proxyIndex.sibling(
                                proxyIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::OPERATOR)),
                            role);
            case 2: {
                QVariant value = ModifyProxyModel::data(
                            proxyIndex.sibling(
                                proxyIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::SECONDATTR)),
                            role);
                return value;
            }
            case 3:
                if (proxyIndex.row() < rowCount(proxyIndex.parent())-1)
                    return ModifyProxyModel::data(
                                proxyIndex.sibling(
                                    proxyIndex.row(),
                                    xmlModel->columnDisplayedAttr(
                                        DBCONDITIONXML::COND,
                                        DBCONDITIONXML::LINKOF)),
                                role);
                else
                    return QVariant();
            default:
                return QVariant();
            }
        }
    }

    if (role == TreeXmlModel::TagRole)
        return ModifyProxyModel::data(proxyIndex.sibling(proxyIndex.row(),0),role);

    if (role == Qt::DecorationRole && proxyIndex.column() == 0) {
        if (data(proxyIndex,TreeXmlModel::TagRole) == DBFILTERBLOCKXML::BLOCK)
            return QIcon(":/block");
        if (data(proxyIndex,TreeXmlModel::TagRole) == DBCONDITIONXML::COND)
            return QIcon(":/expression");
    }

    return QVariant();
}

bool ConditionProxyModel::setData(const QModelIndex &proxyIndex,
                                  const QVariant &value, qint32 role)
{
    if (role == TreeXmlModel::TagRole)
        return ModifyProxyModel::setData(proxyIndex.sibling(proxyIndex.row(),0),value,role);

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        TreeXmlHashModel *xmlModel = qobject_cast<TreeXmlHashModel *>(sourceModel());

        if (data(proxyIndex,TreeXmlModel::TagRole)
                == DBFILTERBLOCKXML::BLOCK) {

            qint32 column;
            switch (proxyIndex.column()) {
            case 3:
                column = xmlModel->columnDisplayedAttr(
                            DBFILTERBLOCKXML::BLOCK,
                            DBFILTERBLOCKXML::LINKOF);
                if (proxyIndex.row() == rowCount(proxyIndex.parent())-1) {
                    if (proxyIndex.row()==0)
                        return ModifyProxyModel::setData(
                                    proxyIndex.sibling(proxyIndex.row(),column),
                                    "И",
                                    role);
                    else
                        return ModifyProxyModel::setData(
                                    proxyIndex.sibling(proxyIndex.row(), column),
                                    data(proxyIndex.sibling(proxyIndex.row()-1, 3),
                                         role),
                                    role);
                } else {
                    return ModifyProxyModel::setData(
                                proxyIndex.sibling(proxyIndex.row(),column),
                                value,
                                role);
                }
            default:
                return false;
            }
        }

        if (data(proxyIndex,TreeXmlModel::TagRole)
                == DBCONDITIONXML::COND) {

            qint32 column;
            switch (proxyIndex.column()) {
            case 0:
                return ModifyProxyModel::setData(
                            proxyIndex.sibling(
                                proxyIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::FIRSTATTR)),
                            value,
                            role);
            case 1:
                return ModifyProxyModel::setData(
                            proxyIndex.sibling(
                                proxyIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::OPERATOR)),
                            value,
                            role);
            case 2:
                return ModifyProxyModel::setData(
                            proxyIndex.sibling(
                                proxyIndex.row(),
                                xmlModel->columnDisplayedAttr(
                                    DBCONDITIONXML::COND,
                                    DBCONDITIONXML::SECONDATTR)),
                            value,
                            role);
            case 3:
                column = xmlModel->columnDisplayedAttr(
                            DBCONDITIONXML::COND,
                            DBCONDITIONXML::LINKOF);

                if (proxyIndex.row() == rowCount(proxyIndex.parent())-1) {
                    if (proxyIndex.row()==0)
                        return ModifyProxyModel::setData(
                                    proxyIndex.sibling(proxyIndex.row(),column),
                                    "И",
                                    role);
                    else
                        return ModifyProxyModel::setData(
                                    proxyIndex.sibling(proxyIndex.row(),column),
                                    data(proxyIndex.sibling(proxyIndex.row()-1, 3), role),
                                    role);
                } else
                    return ModifyProxyModel::setData(
                                proxyIndex.sibling(proxyIndex.row(),column),
                                value,
                                role);
            default:
                return false;
            }

        }
    }

    return ModifyProxyModel::setData(proxyIndex,value,role);
}

bool ConditionProxyModel::insertRows(qint32 row, qint32 count, const QModelIndex &parent)
{
    if (data(parent,TreeXmlModel::TagRole) == DBCONDITIONXML::COND)
        return false;

    return ModifyProxyModel::insertRows(row, count, parent);
}

QModelIndex ConditionProxyModel::insertLastRows(qint32 row, qint32 count, const QModelIndex &parent)
{
    if (data(parent,TreeXmlModel::TagRole) == DBCONDITIONXML::COND)
        return childIdx(-1,-1);

    return ModifyProxyModel::insertLastRows(row, count, parent);
}

}}
