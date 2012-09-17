#ifndef TABLEXMLPROXYMODEL_H
#define TABLEXMLPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "treexmlmodelglobal.h"

class XMLMODELLIB TableXMLProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    TableXMLProxyModel();

    virtual bool filterAcceptsRow (int row, const QModelIndex &parent) const;
    void setFilterIndex(const QModelIndex &parent);
    QModelIndex filterIndex();

private:
    QModelIndex m_index;

};

#endif // TABLEXMLPROXYMODEL_H
