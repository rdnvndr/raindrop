#ifndef FILTERPROPWIDGET_H
#define FILTERPROPWIDGET_H

#include "ui_filterpropwidget.h"
#include "conditionproxymodel.h"
#include <QDataWidgetMapper>
#include <QMenu>
#include <QAction>
#include <treexmlmodel/treexmlhashmodel.h>

//! Диалог редактирования фильтра
/*! Диалог предназначен для редактирования фильтра
*/
class FilterPropWidget : public QWidget, private Ui::FilterPropWidget
{
    Q_OBJECT
    
public:
    //! Конструктор редактора фильтра
    explicit FilterPropWidget(QWidget *parent = 0);

    //! Деструктор редактора фильтра
    virtual ~FilterPropWidget();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

public slots:
    //! Добавление фильтра
    void add();

    //! Удаление фильтра
    void remove();

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

    //! Удаление пустого фильтра
    bool removeEmpty();

    //! Установка текущего фильра
    void setCurrent(const QModelIndex &index);

    //! Возращает находится ли в режиме редактирования
    bool isEdit();

    //! Перевод фильтра в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений фильтра
    void submit();

    //! Отмена изменений фильтра
    void revert();

    //! Удаление фильтра
    void rowsRemoved(const QModelIndex &index,int start,int end);

    //! Установка индексов для делегата ConditionDelegate
    void changeDestClass(const QString& nameClass);

    //! Сообщение о неверном имени фильтра
    void validateFilterName(QValidator::State state) const;

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении текущего фильтра
    void currentIndexChanged(const QModelIndex &index);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Mapper для свойств фильтра
    QDataWidgetMapper* m_mapper;

    //! Хранит индекс предыдущего активного фильтра
    QPersistentModelIndex  m_oldIndex;

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

#endif // FILTERPROPWIDGET_H
