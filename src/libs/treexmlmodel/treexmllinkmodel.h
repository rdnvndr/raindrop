#ifndef TREEXMLLINKMODEL_H
#define TREEXMLLINKMODEL_H

#include <QAbstractProxyModel>
#include "treexmlmodelglobal.h"

namespace RTPTechGroup {
namespace XmlModel {

//! Класс прокси модель дерева, формируемого на основе ссылок
/*! Данный класс предназначен для создания дерева на основе данных TreeXmlHashModel.
 *  Дерево из данных формируется из ссылок, которое описано заранее
*/

class TreeXmlLinkModel : public QAbstractProxyModel
{
    Q_OBJECT
public:
    //! Конструктор модели
    explicit TreeXmlLinkModel(QObject *parent = 0);

    //! Возращает количество столбцов в индексе родителя
    int columnCount (const QModelIndex& parent = QModelIndex()) const;

    //! Возращает количество строк в индексе родителя
    int rowCount(const QModelIndex& parent) const;

    //! Возращает True если имеются потомки
    bool hasChildren(const QModelIndex &parent) const;


    //! Возращает индекс модели для строки и колонки
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const ;

    //! Возращает индекс родителя
    QModelIndex parent (const QModelIndex& index) const ;


    //! Преобразует индекс модели источника в текущую модель
    QModelIndex mapFromSource(const QModelIndex& index) const ;

    //! Преобразует индекс текущей модели в индекс модели источника
    QModelIndex mapToSource(const QModelIndex& index) const ;


    //! Возращает хранимые данные
    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const;

    //! Устанавливает значение для указанной записи
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //! Установка модели источника
    void setSourceModel(QAbstractItemModel *srcModel);

    //! Возвращает флаг записи
    Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Удаление строки
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    //! Вставка строки
    bool insertRows(int  row, int count, const QModelIndex &parent = QModelIndex());

    //! Возвращает true, если имеются потомки у родителей, в противном случае возвращает ложь.
    //! По умолчанию всегда возвращает ложь.
    bool canFetchMore(const QModelIndex &parent) const;
};

}}

#endif // TREEXMLLINKMODEL_H
