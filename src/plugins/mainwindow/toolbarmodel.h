#ifndef TOOLBARMODEL_H
#define TOOLBARMODEL_H

#include <QAbstractItemModel>
#include <QMainWindow>
#include "toolbar.h"

namespace RTPTechGroup {
namespace MainWindow {

//! Модель отображения панелей инструментов в главном окне

class ToolBarModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ToolBarModel(QMainWindow *mainWindow, QObject *parent = 0);
    virtual ~ToolBarModel();

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

    //! Вставка строки
    bool insertRows (qint32 row, qint32 count, const QModelIndex & parent);

    //! Удаление строки
    bool removeRows (qint32 row, qint32 count, const QModelIndex & parent);
    
public slots:
    //! Смена видимости панели инструмента
    void setToolBarVisible(QWidget *widget);

private:
    //! Список панелей инструментов
    QList<ToolBar *> m_toolBars;

    //! Главное окно
    QMainWindow *m_mainWindow;    
};

}
}

#endif // TOOLBARMODEL_H
