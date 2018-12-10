#ifndef ABSTRACTITEMWIDGET_H
#define ABSTRACTITEMWIDGET_H

#include <QWidget>
#include <QDataWidgetMapper>
#include <QTableView>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

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
    virtual void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Получение модели
    virtual RTPTechGroup::XmlModel::TreeXmlHashModel *model();

    //! Получение прокси модели
    virtual RTPTechGroup::XmlModel::TableXMLProxyModel *proxyModel();

    //! Получение данных модели
    virtual QVariant modelData(const QString &tag, const QString &attr,
                               const QModelIndex &index);

    //! Установка таблицы
    virtual void setItemView(QAbstractItemView *itemView);

    //! Возвращает таблицу
    virtual QAbstractItemView *itemView();

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
    virtual void edit(bool flag) = 0;

    //! Перевод в режим редактирования
    virtual void edit();

    //! Перемещение выше
    virtual void up();

    //! Перемещение ниже
    virtual void down();

    //! Установка отображения родительских данных
    virtual void showParent(bool flag,
                            const QString &classTag, const QString &classId,
                            const QString &attrTag,  const QString &attrParent);

    //! Установка родителя
    virtual void setRootIndex(const QModelIndex &index);

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об изменении данных
    void currentIndexChanged(const QModelIndex &index);

private:
    //! Прокси модель
    RTPTechGroup::XmlModel::TableXMLProxyModel *m_proxyModel;

    //! Модель структуры классов
    RTPTechGroup::XmlModel::TreeXmlHashModel *m_model;

    //! Mapper для свойств
    QDataWidgetMapper *m_mapper;

    //! Таблица данных
    QAbstractItemView *m_itemView;

};

}}

#endif // ABSTRACTITEMWIDGET_H
