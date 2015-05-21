#ifndef REFGROUPWIDGET_H
#define REFGROUPWIDGET_H

#include "ui_refgroupwidget.h"

#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QWidget>

#include <treexmlmodel/treexmlhashmodel.h>

#include "abstracteditorwidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования группы справочника
/*! Диалог предназначен для редактирования группы справочника
*/

class RefGroupWidget : public AbstractEditorWidget, private Ui::RefGroupWidget
{
    Q_OBJECT

public:
    //! Конструктор редактора группы справочника
    explicit RefGroupWidget(QWidget *parent = 0);

    //! Деструктор редактора группы справочника
    virtual ~RefGroupWidget();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Проверка запонена ли имя сущности группы справочника
    bool isEmpty();

public slots:
    //! Добавление сущности группы справочника
    void add();

    //! Перевод группы справочника в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений группы справочника
    void submit();

signals:
    //! Сигнал о редактировании группы справочника
    void edited(bool flag);
};

}}

#endif // REFGROUPWIDGET_H
