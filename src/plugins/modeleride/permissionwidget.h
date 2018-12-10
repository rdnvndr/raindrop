#ifndef PERMISSIONWIDGET_H
#define PERMISSIONWIDGET_H

#include "ui_permissionwidget.h"

#include <treexmlmodel/treexmlhashmodel.h>

#include "permissionproxymodel.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования прав доступа
/*! Диалог предназначен для редактирования прав доступа
*/

class PermissionWidget : public QWidget, private Ui::PermissionWidget
{
    Q_OBJECT

public:
    //! Конструктор редактора прав доступа
    explicit PermissionWidget(QWidget *parent = 0);

    //! Деструктор редактора прав доступа
    virtual ~PermissionWidget();

    //! Возвращает модель
    RTPTechGroup::XmlModel::TreeXmlHashModel *model();

    //! Возвращает прокси модели
    PermissionProxyModel *proxyModel();

    //! Установка модели для редактирования прав доступа
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Проверка на возможность удаления
    bool isRemove(const QModelIndex &proxyIndex);

public slots:
    //! Добавление права доступв
    void add();

    //! Удаление права доступв
    void remove();

    //! Установка родителя значений списка
    void setRootIndex(const QModelIndex &index);

    //! Установка отображения родительских атрибутов класса
    void showParent(bool flag);

private slots:
    //! Удаление право доступа без роли
    void currentIndexChange(const QModelIndex &current, const QModelIndex &previous);

    //! Перевод в режим редактирования ячейки дерева
    void cellItemEdit(const QModelIndex& index);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Прокси модель для значений списка
    PermissionProxyModel *m_proxyModel;

    //! Модель структуры классов
    RTPTechGroup::XmlModel::TreeXmlHashModel *m_model;
};

}}

#endif // PERMISSIONWIDGET_H
