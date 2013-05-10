#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include "ui_filterwidget.h"
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlmodel.h>
#include <QDataWidgetMapper>

//! Диалог редактирования фильтров класса
/*! Диалог предназначен для редактирования списка фильтров
    класса
*/

class FilterWidget : public QWidget, private Ui::FilterWidget
{
    Q_OBJECT
    
public:
    //! Конструктор редактора фильтров
    explicit FilterWidget(QWidget *parent = 0);

    //! Деструктор редактора фильтров
    virtual ~FilterWidget();

    //! Установка модели для редактирования составов класса
    void setModel(TreeXMLModel *model);

public slots:
    //! Добавление фильтра класса
    void add();

    //! Удаление фильтра класса
    void remove();

    //! Вызов редактирования фильтра класса
    void edit();

    //! Установка отображения родительских фильтров класса
    void showParent(bool flag);

    //! Установка родителя фильтра
    void setRootIndex(QModelIndex index);

signals:
    //! Сигнал о добавлении данных
    void dataAdded(const QModelIndex &index);

    //! Сигнал о вызове редактирования данных
    void dataEdited(const QModelIndex &index);

    //! Сигнал о изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал о удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал о изменении текущего состава
    void currentIndexChanged(const QModelIndex &index);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Прокси модель для фильтров класса
    TableXMLProxyModel* m_filterModel;

    //! Модель структуры классов
    TreeXMLModel* m_model;

};

#endif // FILTERWIDGET_H
