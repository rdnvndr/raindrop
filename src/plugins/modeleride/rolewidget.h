#ifndef ROLEWIDGET_H
#define ROLEWIDGET_H

#include "ui_rolewidget.h"
#include "abstracteditorwidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования роли
/*! Диалог предназначен для редактирования роли
*/

class RoleWidget : public AbstractEditorWidget, private Ui::RoleWidget
{
    Q_OBJECT

public:
    //! Конструктор редактора роли
    explicit RoleWidget(QWidget *parent = 0);

    //! Деструктор редактора роли
    virtual ~RoleWidget();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Проверка запонена ли имя роли
    bool isEmpty();

public slots:
    //! Добавление роли
    void add();

    //! Перевод роли в режим редактирования
    void edit(bool flag);

    //! Применение изменений роли
    void submit();

    //! Сообщение о неверном имени роли
    void validateRoleName(QValidator::State state) const;

signals:
    //! Сигнал о редактировании группы справочника
    void edited(bool flag);
};

}}

#endif // ROLEWIDGET_H
