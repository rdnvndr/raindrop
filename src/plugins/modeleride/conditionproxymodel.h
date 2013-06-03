#ifndef CONDITIONPROXYMODEL_H
#define CONDITIONPROXYMODEL_H

#include <QAbstractProxyModel>

class ConditionProxyModel : public QAbstractProxyModel
{
public:
    ConditionProxyModel(QObject *parent = 0);

    int columnCount (const QModelIndex& parent = QModelIndex()) const ;
    int rowCount(const QModelIndex& parent) const ;

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const ;
    QModelIndex parent (const QModelIndex& index) const ;

    QModelIndex mapFromSource(const QModelIndex&) const ;
    QModelIndex mapToSource(const QModelIndex&) const ;

    QVariant data(const QModelIndex &proxyIndex, int role) const;
    bool setData(const QModelIndex &proxyIndex, const QVariant &value, int role);

    bool hasChildren(const QModelIndex &parent) const;
};

#endif // CONDITIONPROXYMODEL_H
