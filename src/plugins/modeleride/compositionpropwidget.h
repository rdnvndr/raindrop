#ifndef COMPOSITIONPROPWIDGET_H
#define COMPOSITIONPROPWIDGET_H

#include <QDataWidgetMapper>
#include "ui_compositionpropwidget.h"
#include <treexmlmodel/treexmlmodel.h>

//! Диалог редактирования состава
/*! Диалог предназначен для состава
 */

class CompositionPropWidget : public QWidget, private Ui::CompositionPropWidget
{
    Q_OBJECT
    
public:
    //! Конструктор редактора состава
    explicit CompositionPropWidget(QWidget *parent = 0);

    //! Деструктор редактора состава
    virtual ~CompositionPropWidget();

    //! Установка модели структуры классов
    void setModel(TreeXMLModel *model);

public slots:
    //! Добавление состава
    void add();

    //! Удаление состава
    void remove();

    //! Удаление пустого состава
    void removeEmpty();

    //! Установка текущего состава
    void setCurrent(QModelIndex index);

    //! Перевод состава в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений состава
    void submit();

    //! Отмена изменений состава
    void revert();

    //! Удаление составов
    void rowsRemoved(QModelIndex index,int start,int end);

signals:
    //! Сигнал об изменении данных
    void dataChanged(QModelIndex index);

    //! Сигнал об удалении данных
    void dataRemoved(QModelIndex index);

    //! Сигнал об изменении текущего состава
    void currentIndexChanged(QModelIndex index);

private:
    //! Получение данных модели
    QVariant modelData(QString typeName, QString attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXMLModel* m_model;

    //! Mapper для свойств состава
    QDataWidgetMapper* m_mapper;

    //! Хранит индекс предыдущего активного состава
    int  m_oldIndex;
};

#endif // COMPOSITIONPROPWIDGET_H
