#ifndef PROPLOV_H
#define PROPLOV_H

#include "ui_proplov.h"

#include <treexmlmodel/treexmlhashmodel.h>

#include "abstractpropeditor.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования списка значений
/*! Диалог предназначен для редактирования списка значений
*/

class PropLov : public AbstractPropEditor, private Ui::PropLov
{
    Q_OBJECT

public:
    //! Конструктор диалога редактирования списка значений
    explicit PropLov(QWidget *parent = 0);

    //! Деструктор диалога редактирования списка значений
    virtual ~PropLov();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

public slots:
    //! Установка текущего списка значений
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Перевод списка значений в режим редактирования
    void edit(bool flag);
};

}}

#endif // PROPLOV_H
