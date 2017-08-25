#ifndef ATTRGROUPPROXYMODEL_H
#define ATTRGROUPPROXYMODEL_H

#include <QAbstractListModel>

namespace RTPTechGroup {
namespace ModelerIde {

//! Прокси модель получения списка групп атрибутов
class AttrGroupProxyModel : public QAbstractListModel
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit AttrGroupProxyModel(QObject *parent = 0);

    //! Деструктор класаа
    virtual ~AttrGroupProxyModel();

    //! Возращает количество строк в индексе родителя
    qint32 rowCount(const QModelIndex& parent) const;

    //! Возращает хранимые данные
    QVariant data(const QModelIndex &index, qint32 role = Qt::DisplayRole) const;

    //! Установка модели источника
    void setModel(QAbstractItemModel *srcModel);

    //! Получение модели источника
    QAbstractItemModel *sourceModel() const;

    //! Установка корневого индекса
    void setRootModelIndex(const QModelIndex &index);

    //! Получение корневого индекса
    QModelIndex rootModelIndex();

    //! Указание столбца с уникальными значениями
    void setUniqueColumn(qint32 column);

    //! Возращает столбец с уникальными значениями
    qint32 uniqueColumn() const;

public slots:
    //! Инициализация модели
    void reset();

private:
    qint32 m_uniqueColumn;
    QList<QString> m_list;
    QAbstractItemModel *m_model;
    QPersistentModelIndex m_rootIndex;
};

}}

#endif // ATTRGROUPPROXYMODEL_H
