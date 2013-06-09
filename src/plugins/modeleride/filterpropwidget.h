#ifndef FILTERPROPWIDGET_H
#define FILTERPROPWIDGET_H

#include "ui_filterpropwidget.h"
#include "conditionproxymodel.h"
#include <QDataWidgetMapper>
#include <treexmlmodel/treexmlmodel.h>

class FilterPropWidget : public QWidget, private Ui::FilterPropWidget
{
    Q_OBJECT
    
public:
    //! Конструктор редактора фильтра
    explicit FilterPropWidget(QWidget *parent = 0);

    //! Деструктор редактора фильтра
    virtual ~FilterPropWidget();

    //! Установка модели структуры классов
    void setModel(TreeXMLModel *model);

public slots:
    //! Добавление фильтра
    void add();

    //! Удаление фильтра
    void remove();

    //! Добавление условие фильтра
    void addCondition();

    //! Удаление условия фильтра
    void removeCondition();

    //! Добавление блока фильтра
    void addBlock();

    //! Удаление пустого фильтра
    void removeEmpty();

    //! Установка текущего фильра
    void setCurrent(const QModelIndex &index);

    //! Перевод фильтра в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений фильтра
    void submit();

    //! Отмена изменений фильтра
    void revert();

    //! Удаление фильтра
    void rowsRemoved(const QModelIndex &index,int start,int end);

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
    TreeXMLModel* m_model;

    //! Mapper для свойств фильтра
    QDataWidgetMapper* m_mapper;

    //! Хранит индекс предыдущего активного фильтра
    int  m_oldIndex;

    //! Модель редактора условий
    ConditionProxyModel *m_conditionModel;
};

#endif // FILTERPROPWIDGET_H
