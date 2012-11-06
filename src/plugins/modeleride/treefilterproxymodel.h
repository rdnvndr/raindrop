#ifndef TREEFILTERPROXYMODEL_H
#define TREEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QStringList>
#include <QSet>

//! Фильрующая древовидная прокси модель
/*! Прокси модель дерева. предназначена для сортировки и фильтрации
 */

class TreeFilterProxyModel : public QSortFilterProxyModel
{
public:
    //! Конструктор прокси модели
    TreeFilterProxyModel();

    //! Добавление скрытых тэгов
    void addHiddenTag(QString tag);

    //! Удаление скрытых тэгов
    void removeHiddenTag(QString tag);

    //! Список скрытых тэгов
    QSet<QString> hiddenTags();

protected:
    //! Фильтр строчек
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    //! Фильтр строчек по дереву
    bool filterAcceptsRowItself(int source_row, const QModelIndex &source_parent) const;

    //! Поиск детей удолетворящих фильтру
    bool hasAcceptedChildren(int source_row, const QModelIndex &source_parent) const;

    //! Сортировка. Порядок: Класс, Состав, Атрибут, Алфавит
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    //! Список тэгов для фильтрации
    QSet<QString> m_filterTags;
};

#endif // TREEFILTERPROXYMODEL_H
