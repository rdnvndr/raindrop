#ifndef REFWIDGET_H
#define REFWIDGET_H

#include "ui_refwidget.h"

#include <QDataWidgetMapper>
#include <QMessageBox>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

#include "abstracteditorwidget.h"

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования справочника
/*! Диалог предназначен для редактирования справочника
 */

class RefWidget : public AbstractEditorWidget, private Ui::RefWidget
{
    Q_OBJECT

public:
    //! Конструктор редактора справочника
    explicit RefWidget(QWidget *parent = 0);

    //! Деструктор редактора справочника
    virtual ~RefWidget();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Проверка запонено ли имя справочника
    bool isEmpty();

public slots:
    //! Добавление справочника
    void add();

    //! Перевод справочника в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений справочника
    void submit();

signals:

    //! Сигнал о редактировании справочника
    void edited(bool flag);
};

}}

#endif // REFWIDGET_H
