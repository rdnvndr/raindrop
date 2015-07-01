#ifndef PROPENTITYGROUP_H
#define PROPENTITYGROUP_H

#include "ui_propentitygroup.h"

#include <QDataWidgetMapper>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>

#include "abstractpropeditor.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования группы величины
/*! Диалог предназначен для редактирования группы величины
*/
class PropEntityGroup : public AbstractPropEditor, private Ui::PropEntityGroup
{
    Q_OBJECT

public:
    //! Конструктор диалога редактирования группы величины
    explicit PropEntityGroup(QWidget *parent = 0);

    //! Деструктор диалога редактирования группы величины
    virtual ~PropEntityGroup();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Получение индетификатора по индексу
    QString dataId(const QModelIndex &index);

public slots:
    //! Установка текущей группы величины
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Перевод группы величины в режим редактирования
    void edit(bool flag = true);
};

}}

#endif // PROPENTITYGROUP_H
