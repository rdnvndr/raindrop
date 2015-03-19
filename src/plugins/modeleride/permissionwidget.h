#ifndef PERMISSIONWIDGET_H
#define PERMISSIONWIDGET_H

#include "ui_permissionwidget.h"

#include <treexmlmodel/treexmlhashmodel.h>

#include "permissionproxymodel.h"

using namespace RTPTechGroup::XmlModel;

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

    //! Возращает модель
   TreeXmlHashModel *model();

   //! Возращает прокси модели
   PermissionProxyModel* proxyModel();

    //! Установка модели для редактирования прав доступа
    void setModel(TreeXmlHashModel *model);

    //! Проверка на возможность удаления
    bool isRemove(const QModelIndex &proxyIndex);

signals:
    //! Сигнал об изменении корневого индекса в прокси
    void proxyIndexChanged(const QModelIndex &index);

public slots:
    //! Добавление права доступв
    void add();

    //! Удаление права доступв
    void remove();

    //! Перевод в режим редактирования
    void edit(bool flag = true);

    //! Установка родителя значений списка
    void setRootIndex(const QModelIndex &index);

private:

    //! Прокси модель для значений списка
    PermissionProxyModel* m_proxyModel;

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Виджет отображения данных
    QAbstractItemView *m_itemView;
};

}}

#endif // PERMISSIONWIDGET_H
