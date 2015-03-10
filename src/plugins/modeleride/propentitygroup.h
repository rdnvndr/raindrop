#ifndef PROPENTITYGROUP_H
#define PROPENTITYGROUP_H

#include <QDataWidgetMapper>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>

#include "ui_propentitygroup.h"
#include "abstractpropeditor.h"

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования группы сущности ЕИ
/*! Диалог предназначен для редактирования группы сущности ЕИ
*/
class PropEntityGroup : public AbstractPropEditor, private Ui::PropEntityGroup
{
    Q_OBJECT

public:
    //! Конструктор диалога редактирования группы сущности ЕИ
    explicit PropEntityGroup(QWidget *parent = 0);

    //! Деструктор диалога редактирования группы сущности ЕИ
    virtual ~PropEntityGroup();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

public slots:
    //! Установка текущей группы сущности ЕИ
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Перевод группы сущности ЕИ в режим редактирования
    void edit(bool flag = true);
};

}}

#endif // PROPENTITYGROUP_H
