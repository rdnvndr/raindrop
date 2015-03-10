#ifndef PROPREFGROUP_H
#define PROPREFGROUP_H

#include <QDataWidgetMapper>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>

#include "ui_proprefgroup.h"
#include "abstractpropeditor.h"

namespace RTPTechGroup {
namespace ModelerIde {

using namespace RTPTechGroup::XmlModel;

//! Диалог редактирования группы справочника
/*! Диалог предназначен для редактирования группы справочника
*/

class PropRefGroup : public AbstractPropEditor, private Ui::PropRefGroup
{
    Q_OBJECT

public:
    //! Конструктор диалога редактирования группы справочника
    explicit PropRefGroup(QWidget *parent = 0);

    //! Деструктор диалога редактирования группы справочника
    virtual ~PropRefGroup();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

public slots:
    //! Установка текущей группы справочника
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Перевод группы справочника в режим редактирования
    void edit(bool flag = true);
};

}}

#endif // PROPREFGROUP_H
