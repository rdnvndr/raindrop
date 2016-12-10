#ifndef LOVVALUEWIDGET_H
#define LOVVALUEWIDGET_H

#include "ui_lovvaluewidget.h"

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>

#include "abstractmodifywidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования значений списка
/*! Диалог предназначен для редактирования значений списка
*/

class LovValueWidget : public AbstractModifyWidget, private Ui::LovValueWidget
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit LovValueWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~LovValueWidget();

    //! Установка модели для редактирования значений списка
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

public slots:
    //! Добавление значений списка
    void add();

    //! Перевод в режим редактирования
    void edit(bool flag);
};

}}

#endif // LOVVALUEWIDGET_H
