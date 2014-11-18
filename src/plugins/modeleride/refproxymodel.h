#ifndef REFPROXYMODEL_H
#define REFPROXYMODEL_H

#include <treexmlmodel/modifyproxymodel.h>
#include <treexmlmodel/treexmlmodel.h>

class RefProxyModel : public ModifyProxyModel
{
    Q_OBJECT
public:
    explicit RefProxyModel(QObject *parent = 0);

    //! Получение данных модели
    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const;
};

#endif // REFPROXYMODEL_H
