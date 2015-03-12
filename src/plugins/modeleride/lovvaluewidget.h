#ifndef LOVVALUEWIDGET_H
#define LOVVALUEWIDGET_H

#include "ui_lovvaluewidget.h"

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>

#include "abstractlisteditorwidget.h"

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования значений списка
/*! Диалог предназначен для редактирования значений списка
*/

class LovValueWidget : public AbstractListEditorWidget, private Ui::LovValueWidget
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit LovValueWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~LovValueWidget();

    //! Установка модели для редактирования значений списка
    void setModel(TreeXmlHashModel *model);

public slots:
    //! Добавление значений списка
    void add();

    //! Перевод в режим редактирования
    void edit(bool flag = true);
};

}}

#endif // LOVVALUEWIDGET_H
