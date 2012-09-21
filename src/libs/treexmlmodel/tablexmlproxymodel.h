#ifndef TABLEXMLPROXYMODEL_H
#define TABLEXMLPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QStringList>
#include "treexmlmodelglobal.h"

class XMLMODELLIB TableXMLProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    TableXMLProxyModel();

    virtual bool filterAcceptsRow (int row, const QModelIndex &parent) const;
    void setFilterIndex(const QModelIndex &parent);
    QModelIndex filterIndex();

    QStringList attributeTags(QStringList list);
    void setAttributeTags(QStringList list);
private:
    QModelIndex m_index;
    QStringList m_tags;

};

#endif // TABLEXMLPROXYMODEL_H
