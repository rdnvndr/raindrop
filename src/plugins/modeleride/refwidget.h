#ifndef REFWIDGET_H
#define REFWIDGET_H

#include "ui_refwidget.h"

#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QValidator>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

#include "abstracteditorwidget.h"

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
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Проверка запонено ли имя справочника
    bool isEmpty();

public slots:
    //! Добавление справочника
    bool add();

    //! Перевод справочника в режим редактирования
    void edit(bool flag);

    //! Применение изменений справочника
    void submit();

    //! Сообщение о неверном имени справочника
    void validateRefName(QValidator::State state) const;

signals:

    //! Сигнал о редактировании справочника
    void edited(bool flag);
};

}}

#endif // REFWIDGET_H
