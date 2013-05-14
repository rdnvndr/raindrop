#ifndef CONDITIONPROXYMODEL_H
#define CONDITIONPROXYMODEL_H

#include <QIdentityProxyModel>

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
};

#endif // CONDITIONPROXYMODEL_H
