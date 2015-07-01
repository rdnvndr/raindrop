#ifndef ENTITYGROUPWIDGET_H
#define ENTITYGROUPWIDGET_H

#include "ui_entitygroupwidget.h"

#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QWidget>

#include <treexmlmodel/treexmlhashmodel.h>

#include "abstracteditorwidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования группы величины
/*! Диалог предназначен для редактирования группы величины
*/

class EntityGroupWidget : public AbstractEditorWidget, private Ui::EntityGroupWidget
{
    Q_OBJECT

public:
    //! Конструктор редактора группы величины
    explicit EntityGroupWidget(QWidget *parent = 0);

    //! Деструктор редактора группы величины
    virtual ~EntityGroupWidget();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Проверка запонена ли имя группы величины
    bool isEmpty();

public slots:
    //! Добавление группы величины
    void add();

    //! Перевод группы величины в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений группы величины
    void submit();

signals:
    //! Сигнал о редактировании группы величины
    void edited(bool flag);
};

}}

#endif // ENTITYGROUPWIDGET_H
