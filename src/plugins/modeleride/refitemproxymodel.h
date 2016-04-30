#ifndef REFITEMPROXYMODEL_H
#define REFITEMPROXYMODEL_H

#include <QSortFilterProxyModel>

namespace RTPTechGroup {
namespace ModelerIde {

//! Класс прокси модель для отображения справочников для указанного класса

class RefItemProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:

    //! Конструктор модели
    explicit RefItemProxyModel(QObject *parent = 0);

    //! Метод который определяет отображать ли указанную строку
    virtual bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const;

    //! Метод который определяет сортировку
    virtual bool lessThan(const QModelIndex & left, const QModelIndex & right) const;

    //! Установка идентификатора класса по которому сортируются справочники
    void setClassId(const QString &classId);

protected:
    //! Поиск детей удолетворящих фильтру
    bool hasAcceptedChildren(int source_row, const QModelIndex &source_parent) const;

    //! Фильтр по дереву
    bool filterAcceptsRowItself(int source_row, const QModelIndex &source_parent) const;

private:
    QString m_classId;
};

}}

#endif
