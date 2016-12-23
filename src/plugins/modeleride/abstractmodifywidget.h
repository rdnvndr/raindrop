#ifndef ABSTRACTMODIFYWIDGET_H
#define ABSTRACTMODIFYWIDGET_H

#include <QWidget>
#include <QAbstractProxyModel>
#include <QAbstractItemView>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования при помощи списка с возможностью отмены изменений
/*! Диалог предназначен для редактирования при помощи списка
 *  с возможностью отмены изменений
 */

class AbstractModifyWidget : public QWidget
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit AbstractModifyWidget(QWidget *parent = 0);

    //! Деструктор класса
    ~AbstractModifyWidget();

    //! Установка модели для редактирования
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

     //! Возращает модель
    RTPTechGroup::XmlModel::TreeXmlHashModel *model();

    //! Возращает прокси модели
    RTPTechGroup::XmlModel::ModifyProxyModel *proxyModel();

    //! Проверка на возможность удаления
    virtual bool isRemove(const QModelIndex &proxyIndex);

    //! Установка виджета отображения данных
    virtual void setItemView(QAbstractItemView *itemView);

    //! Возращает виджет отображения данных
    virtual QAbstractItemView *itemView();

signals:
    //! Сигнал об изменении корневого индекса в прокси
    void proxyIndexChanged(const QModelIndex &index);

public slots:
    //! Добавление значений списка
    virtual bool add(const QString &tag);

    //! Удаление значений списка
    virtual void remove();

    //! Применение изменений значений списка
    virtual void submit();

    //! Перевод в режим редактирования
    virtual void edit(bool flag);

    //! Перевод в режим редактирования
    virtual void edit();

    //! Отмена изменений значений списка
    virtual void revert();

    //! Установка родителя значений списка
    virtual void setRootIndex(const QModelIndex &index);

    //! Перемещение выше
    virtual void up();

    //! Перемещение ниже
    virtual void down();

private:

    //! Прокси модель для значений списка
    RTPTechGroup::XmlModel::ModifyProxyModel *m_proxyModel;

    //! Модель структуры классов
    RTPTechGroup::XmlModel::TreeXmlHashModel *m_model;

    //! Виджет отображения данных
    QAbstractItemView *m_itemView;
};

}}

#endif // ABSTRACTMODIFYWIDGET_H
