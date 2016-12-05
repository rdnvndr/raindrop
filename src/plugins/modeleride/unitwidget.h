#ifndef UNITWIDGET_H
#define UNITWIDGET_H

#include "ui_unitwidget.h"

#include <QStringListModel>
#include <QDataWidgetMapper>

#include <treexmlmodel/modifyproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

#include "abstractmodifywidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования ЕИ
/*! Диалог предназначен для редактирования ЕИ
*/
class UnitWidget : public AbstractModifyWidget, private Ui::UnitWidget
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit UnitWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~UnitWidget();

    //! Установка модели для редактирования ЕИ
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

public slots:
    //! Добавление ЕИ
    void add();

    //! Перевод в режим редактирования
    void edit(bool flag);
};

}}

#endif // UNITWIDGET_H
