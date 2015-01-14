#include "attrgroupproxymodel.h"
#include <QDebug>

AttrGroupProxyModel::AttrGroupProxyModel(QObject *parent)
:TableXMLProxyModel(parent)
{
    setUniqueColumn(-1);
}

AttrGroupProxyModel::~AttrGroupProxyModel()
{

}

void AttrGroupProxyModel::setUniqueColumn(int column)
{
    m_uniqueColumn = column;
}

int AttrGroupProxyModel::uniqueColumn() const
{
    return m_uniqueColumn;
}

bool AttrGroupProxyModel::filterAcceptsRow(int row, const QModelIndex &parent) const
{
    if (TableXMLProxyModel::filterAcceptsRow(row, parent)) {
        QModelIndex right = sourceModel()->index(row, uniqueColumn(), parent);
        if (right.isValid()) {
            QVariant rightData = right.data();
            int i = row;
            while (i>0) {
                QModelIndex left =  sourceModel()->index(--i, uniqueColumn(), parent);
                if (left.data() == rightData)
                    return false;
            }
            return true;
        }
    }
    return false;
}
