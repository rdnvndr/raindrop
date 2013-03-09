#ifndef TOOLBARMODEL_H
#define TOOLBARMODEL_H

#include <QAbstractItemModel>
#include <QMainWindow>
#include <QSignalMapper>
#include "toolbar.h"

//! Модель отображения панелей инструментов в главном окне

class ToolBarModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ToolBarModel(QMainWindow *mainWindow, QObject *parent = 0);
    virtual ~ToolBarModel();

    //! Возращает хранимые данные
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole ) const;

    //! Устанавливает значение
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

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

    //! Вставка строки
    bool insertRows (int row, int count, const QModelIndex & parent);

    //! Удаление строки
    bool removeRows (int row, int count, const QModelIndex & parent);

    
signals:
    
public slots:
    //! Смена видимости панели инструмента
    void setToolBarVisible(QWidget *widget);

private:
    //! Список панелей инструментов
    QList<ToolBar *> m_toolBars;

    //! Маппер сигналов для панелей инструментов
    QSignalMapper *m_mapper;

    //! Главное окно
    QMainWindow *m_mainWindow;
    
};

#endif // TOOLBARMODEL_H
