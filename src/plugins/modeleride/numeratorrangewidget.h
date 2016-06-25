#ifndef NUMERATORRANGEWIDGET_H
#define NUMERATORRANGEWIDGET_H

#include "ui_numeratorrangewidget.h"

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>

#include "abstractmodifywidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования интервалов нумератора
/*! Диалог предназначен для редактирования интервалов нумератора
*/

class NumeratorRangeWidget : public AbstractModifyWidget, private Ui::NumeratorRangeWidget
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit NumeratorRangeWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~NumeratorRangeWidget();

    //! Установка модели для редактирования интервалов нумератора
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

public slots:
    //! Добавление интервала нумератора
    void add();

    //! Перевод в режим редактирования
    void edit(bool flag = true);
};

}}

#endif // NUMERATORRANGEWIDGET_H
