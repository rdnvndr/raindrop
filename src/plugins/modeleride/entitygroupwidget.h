#ifndef ENTITYGROUPWIDGET_H
#define ENTITYGROUPWIDGET_H

#include "ui_entitygroupwidget.h"

#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QWidget>

#include <treexmlmodel/treexmlhashmodel.h>

#include "abstracteditorwidget.h"

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования группы сущности ЕИ
/*! Диалог предназначен для редактирования группы сущности ЕИ
*/

class EntityGroupWidget : public AbstractEditorWidget, private Ui::EntityGroupWidget
{
    Q_OBJECT

public:
    //! Конструктор редактора группы сущности ЕИ
    explicit EntityGroupWidget(QWidget *parent = 0);

    //! Деструктор редактора группы сущности ЕИ
    virtual ~EntityGroupWidget();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Проверка запонена ли имя сущности группы сущности ЕИ
    bool isEmpty();

public slots:
    //! Добавление сущности группы сущности ЕИ
    void add();

    //! Перевод группы сущности ЕИ в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений группы сущности ЕИ
    void submit();

signals:
    //! Сигнал о редактировании группы сущности ЕИ
    void edited(bool flag);
};

}}

#endif // ENTITYGROUPWIDGET_H
