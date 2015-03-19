#ifndef PERMISSIONPROXYMODEL_H
#define PERMISSIONPROXYMODEL_H

#include <QSortFilterProxyModel>

namespace RTPTechGroup {
namespace ModelerIde {

//! Прокси модель ролей класса
/*! Прокси модель предназначена для редактирования ролей класса
 */

class PermissionProxyModel : public QSortFilterProxyModel
{
public:
    PermissionProxyModel();
    ~PermissionProxyModel();

    void setRootIndex(const QModelIndex& index);
    QModelIndex rootIndex();

protected:
    //! Фильтр строчек
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    //! Сортировка. Порядок: Класс, Состав, Атрибут, Алфавит
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    QModelIndex m_rootIndex;
};

}}

#endif // PERMISSIONPROXYMODEL_H
