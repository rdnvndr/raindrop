#ifndef REFITEMPROXYMODEL_H
#define REFITEMPROXYMODEL_H

#include <QAbstractProxyModel>
#include <QSet>

namespace RTPTechGroup {
namespace ModelerIde {

//! Класс прокси модель

class RefItemProxyModel : public QAbstractProxyModel
{
    Q_OBJECT

public:

    //! Конструктор модели
    explicit RefItemProxyModel(QObject *parent = 0);


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

    //! Возращает название заголовка
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    //! Устанавливает название заголовка
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole);


    //! Удаление строки
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    //! Вставка строки
    bool insertRows(int  row, int count, const QModelIndex &parent = QModelIndex());

    //! Возвращает индекс модели buddy для элемента чей индекс передан параметром.
    //! Когда пользователь захочет отредактировать элемент, view (представление)
    //! вызывет эту функцию, чтобы проверить должен ли быть отредактирован другой
    //! элемент этой модели. Тогда view создаст делегат, используя индекс buddy элемента.
    //! По умолчанию каждый элемент является собственным buddy.
    QModelIndex buddy(const QModelIndex &index) const;

    //! Возвращает true, если имеются потомки у родителей, в противном случае возвращает ложь.
    //! По умолчанию всегда возвращает ложь.
    bool canFetchMore(const QModelIndex &parent) const;

public slots:

    //! Слот обработки сигнала изменения данных источника
    void sourceDataChanged(const QModelIndex &left, const QModelIndex &right);

    //! Слот обработки сигнала удаления строк в источнике
    void sourceRowsRemoved(const QModelIndex &parent, int start, int end);

    //! Слот обработки сигнала перед удалением строк в источника
    void sourceRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);

    //! Слот обработки сигнала вставки строк в источнике
    void sourceRowsInserted(const QModelIndex &parent, int start, int end);

private:

    //! Список названий заголовков
    QMap<int, QString> m_header;
};

}}

#endif
