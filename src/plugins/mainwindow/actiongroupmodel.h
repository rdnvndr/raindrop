#ifndef ACTIONGROUPMODEL_H
#define ACTIONGROUPMODEL_H

#include <QAbstractItemModel>
#include <QAction>

class ActionGroupModel : public QAbstractItemModel
{
    Q_OBJECT
public:
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

    void setActionGroup(QMultiHash<QString, QAction *> *actions);
    
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
signals:
    
public slots:

private:
    QMultiHash <QString, QAction *> *m_actions;
    
};

#endif // ACTIONGROUPMODEL_H
