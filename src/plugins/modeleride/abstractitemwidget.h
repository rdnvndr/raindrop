#ifndef ABSTRACTITEMWIDGET_H
#define ABSTRACTITEMWIDGET_H

#include <QWidget>
#include <QDataWidgetMapper>
#include <QTableView>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {


//! Диалог редактирования при помощи списка
/*! Диалог предназначен для редактирования при помощи списка
 */

class AbstractItemWidget : public QWidget
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit AbstractItemWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~AbstractItemWidget();

    //! Установка модели
    virtual void setModel(TreeXmlHashModel *model);

    //! Получение модели
    virtual TreeXmlHashModel *model();

    //! Получение прокси модели
    virtual TableXMLProxyModel *tableModel();

    //! Получение данных модели
    virtual QVariant modelData(const QString &tag, const QString &attr,
                               const QModelIndex &index);

    //! Установка таблицы
    virtual void setTableView(QTableView *tableView);

    //! Возращает таблицу
    virtual QTableView *tableView();

    //! Получение маппера данных
    virtual QDataWidgetMapper *dataMapper();

public slots:
    //! Добавление строки
    virtual bool add(const QString& tag);

    //! Удаление строки
    virtual void remove();

    //! Установка текущей строки
    virtual void setCurrent(const QModelIndex &index);

    //! Применение изменений
    virtual void submit();

    //! Отмена изменений
    virtual void revert();

    //! Проверка находится ли в режиме редактирования
    virtual bool isEdit() = 0;

    //! Перевод в режим редактирования
    virtual void edit(bool flag = true) = 0;

    //! Установка родителя
    virtual void setRootIndex(QModelIndex index);

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении данных
    void currentIndexChanged(const QModelIndex &index);

private:
    //! Прокси модель
    TableXMLProxyModel* m_tableModel;

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Mapper для свойств
    QDataWidgetMapper* m_mapper;

    //! Таблица данных
    QTableView *m_tableView;

};

}}

#endif // ABSTRACTITEMWIDGET_H
