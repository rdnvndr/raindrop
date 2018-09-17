#ifndef ACTIONGROUPMODEL_H
#define ACTIONGROUPMODEL_H

#include <QAbstractItemModel>
#include <QAction>



namespace RTPTechGroup {
namespace MainWindow {

//! Модель сгруппированных QAction
/*! Модель сгруппированных QAction представляет собой на первом уровне
 *  список групп, а на втором уровне список QAction. \n
    Пример:
    \code
        ActionGroupModel *model = new ActionGroupModel(actions);
        listCategory->setModel(model);
        listCommands->setModel(model);

        listCategory->setCurrentIndex(model->index(0,0));
        listCommands->setRootIndex(model->index(0,0));
    \endcode
*/
class ActionGroupModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    //! Конструктор
    explicit ActionGroupModel(QMultiHash<QString, QAction *> *actions);

    //! Возвращает хранимые данные
    QVariant data(const QModelIndex &index, qint32 role = Qt::DisplayRole ) const;

    //! Устанавливает значение
    bool setData(const QModelIndex &index, const QVariant &value, qint32 role = Qt::EditRole);

    //! Возвращает флаг записи
    Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Возвращает название заголовка
    QVariant headerData(qint32 section, Qt::Orientation orientation,
                        qint32 role = Qt::DisplayRole) const;

    //! Возвращает индекс модели для строки и колонки
    QModelIndex index(qint32 row, qint32 column,
                      const QModelIndex &parent = QModelIndex()) const;

    //! Возвращает индекс родителя
    QModelIndex parent(const QModelIndex &child) const;

    //! Возвращает количество строк в индексе родителя
    qint32 rowCount(const QModelIndex &parent = QModelIndex()) const;

    //! Возвращает количество столбцов в индексе родителя
    qint32 columnCount(const QModelIndex &parent = QModelIndex()) const;

    //! Указывает данные для формирования модели
    void setActionGroup(QMultiHash<QString, QAction *> *actions);
    
    //! Разрешенные действия для Drag
    Qt::DropActions supportedDragActions() const;

    //! Разрешенные действия для Drop
    Qt::DropActions supportedDropActions() const;

    //! Возвращает типы данных перетаскивания
    QStringList mimeTypes() const;

    //! Упаковывает данные для перетаскивания
    QMimeData *mimeData(const QModelIndexList &indexes) const;

private:
    //! Данные для формирования модели
    QMultiHash <QString, QAction *> *m_actions;
    
};
}
}
#endif // ACTIONGROUPMODEL_H
