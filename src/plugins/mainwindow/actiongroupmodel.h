#ifndef ACTIONGROUPMODEL_H
#define ACTIONGROUPMODEL_H

#include <QAbstractItemModel>
#include <QAction>

class ActionGroupModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    //! Конструктор
    explicit ActionGroupModel(QMultiHash<QString, QAction *> *actions);

    //! Возращает хранимые данные
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole ) const;

    //! Возвращает флаг записи
    Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Возращает название заголовка
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    //! Возращает индекс модели для строки и колонки
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;

    //! Возращает индекс родителя
    QModelIndex parent(const QModelIndex &child) const;

    //! Возращает количество строк в индексе родителя
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //! Возращает количество столбцов в индексе родителя
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //! Указывает данные для формирования модели
    void setActionGroup(QMultiHash<QString, QAction *> *actions);
    
    //! Разрешенные действия для Drag
    Qt::DropActions supportedDragActions() const;

    //! Разрешенные действия для Drop
    Qt::DropActions supportedDropActions() const;

    //! Возращает типы данных перетаскивания
    QStringList mimeTypes() const;

    //! Упаковывает данные для перетаскивания
    QMimeData *mimeData(const QModelIndexList &indexes) const;

private:
    //! Данные для формирования модели
    QMultiHash <QString, QAction *> *m_actions;
    
};

#endif // ACTIONGROUPMODEL_H
