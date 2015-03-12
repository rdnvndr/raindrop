#ifndef FILTERPROPWIDGET_H
#define FILTERPROPWIDGET_H

#include "ui_filterpropwidget.h"

#include <QDataWidgetMapper>
#include <QMenu>
#include <QAction>

#include <treexmlmodel/treexmlhashmodel.h>

#include "conditionproxymodel.h"
#include "abstracteditorwidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования фильтра
/*! Диалог предназначен для редактирования фильтра
*/
class FilterPropWidget : public AbstractEditorWidget, private Ui::FilterPropWidget
{
    Q_OBJECT
    
public:
    //! Конструктор редактора фильтра
    explicit FilterPropWidget(QWidget *parent = 0);

    //! Деструктор редактора фильтра
    virtual ~FilterPropWidget();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Проверка запонены ли данные
    virtual bool isEmpty();

public slots:
    //! Добавление фильтра
    void add();

    //! Добавление выражения фильтра
    void addCondition();

    //! Добавление подвыражение фильтра
    void addSubCondition();

    //! Удаление условия фильтра
    void removeCondition();

    //! Добавление блока фильтра
    void addBlock();

    //! Добавление подблока фильтра
    void addSubBlock();

    //! Установка текущего фильра
    void setCurrent(const QModelIndex &index);

    //! Возращает находится ли в режиме редактирования
    bool isEdit();

    //! Перевод фильтра в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений фильтра
    void submit();

    //! Установка индексов для делегата ConditionDelegate
    void changeDestClass(const QString& nameClass);

    //! Сообщение о неверном имени фильтра
    void validateFilterName(QValidator::State state) const;

private:
    //! Модель редактора условий
    ConditionProxyModel *m_conditionModel;

    //! Меню добавить условие
    QMenu *menuAddCondition;

    //! Действие добавить блок
    QAction *actionAddBlock;

    //! Действие добавить выражение
    QAction *actionAddCondition;

    //! Действие добавить подблок
    QAction *actionAddSubBlock;

    //! Действие добавить подвыражение
    QAction *actionAddSubCondition;
};

}}

#endif // FILTERPROPWIDGET_H
