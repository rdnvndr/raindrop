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
    virtual bool filterAcceptsRow (qint32 source_row, const QModelIndex &source_parent) const;

    //! Метод который определяет сортировку
    virtual bool lessThan(const QModelIndex & left, const QModelIndex & right) const;

    //! Установка индекса класса по которому отображаются справочники
    void setClassIndex(QModelIndex &idx);

    //! Получение индекса класса по которому отображаются справочники
    QModelIndex classIndex();

    //! Получение флага рекурсивного обхода классов
    bool recursion() const;

    //! Установка рекурсивного обхода классов
    void setRecursion(bool recursion);

protected:
    //! Поиск класса удолетворяющего фильтру
    bool hasAcceptedChildren(const QModelIndex &link_index, const QModelIndex &source_parent) const;

    //! Фильтр по дереву
    bool filterAcceptsRowItself(qint32 source_row, const QModelIndex &source_parent) const;

    //! Возвращает дочерний индекс
    QModelIndex childIdx(int arow, int acolumn, const QModelIndex &parent = QModelIndex()) const;

private:
    QPersistentModelIndex m_classIndex;
    bool m_recursion;
};

}}

#endif
