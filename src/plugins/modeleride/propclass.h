#ifndef PROPCLASS_H
#define PROPCLASS_H

#include "ui_propclass.h"

#include <QDataWidgetMapper>
#include <QStringListModel>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>

#include "abstractpropeditor.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования класса
/*! Диалог предназначен для редактирования класса. Включает в себя диалоги
    редактирования свойств класса, списка атрибутов и списка составов
 */

class PropClass : public AbstractPropEditor, public Ui::PropClass
{
    Q_OBJECT
    
public:
    //! Конструктор диалога редактирования классов
    explicit PropClass(QWidget *parent = 0);

    //! Деструктор диалога редактирования классов
    virtual ~PropClass();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Получение индетификатора по индексу
    QString dataId(const QModelIndex &index);

signals:
    //! Сигнал вызова редактирования состава
    void editComposition(const QModelIndex &index);

    //! Сигнал вызова редактирования фильтра
    void editFilter(const QModelIndex &index);

public slots:
    //! Установка текущего класса
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Перевод в режим редактирования
    virtual void edit(bool flag  = true);

    //! Вызов редактирования состава
    void onEditComposition(const QModelIndex &index);

    //! Вызов редактирования фильтра
    void onEditFilter(const QModelIndex &index);
};

}}

#endif // PROPCLASS_H
