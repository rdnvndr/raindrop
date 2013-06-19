#ifndef CONDITIONPROXYMODEL_H
#define CONDITIONPROXYMODEL_H

#include <QAbstractProxyModel>
#include <treexmlmodel/modifyproxymodel.h>

class ConditionProxyModel : public ModifyProxyModel
{
    Q_OBJECT
public:
    ConditionProxyModel(QObject *parent = 0);

    int columnCount (const QModelIndex& parent = QModelIndex()) const ;

    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &proxyIndex, const QVariant &value,
                 int role = Qt::EditRole);
    bool insertRows(int row, int count, const QModelIndex &parent);
};

#endif // CONDITIONPROXYMODEL_H
