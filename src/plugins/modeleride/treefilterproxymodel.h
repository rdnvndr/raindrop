#ifndef TREEFILTERPROXYMODEL_H
#define TREEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QStringList>
#include <QSet>

class TreeFilterProxyModel : public QSortFilterProxyModel
{
public:
    TreeFilterProxyModel();
    void addHiddenTag(QString tag);
    void removeHiddenTag(QString tag);
    QSet<QString> hiddenTags();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool filterAcceptsRowItself(int source_row, const QModelIndex &source_parent) const;
    bool hasAcceptedChildren(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    //! Список тэгов для фильтрации
    QSet<QString> m_filterTags;
};

#endif // TREEFILTERPROXYMODEL_H
