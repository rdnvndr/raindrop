#ifndef COMPOSITIONPROPWIDGET_H
#define COMPOSITIONPROPWIDGET_H

#include <QDataWidgetMapper>
#include "ui_compositionpropwidget.h"
#include <treexmlmodel/treexmlhashmodel.h>

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
    void setModel(TreeXmlHashModel *model);

public slots:
    //! Добавление состава
    void add();

    //! Удаление состава
    void remove();

    //! Удаление пустого состава
    void removeEmpty();

    //! Установка текущего состава
    void setCurrent(const QModelIndex &index);

    //! Возращает находится ли в режиме редактирования
    bool isEdit();

    //! Перевод состава в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений состава
    void submit();

    //! Отмена изменений состава
    void revert();

    //! Удаление составов
    void rowsRemoved(const QModelIndex &index,int start,int end);

    //! Сообщение о неверном имени состава
    void validateCompositionName(QValidator::State state) const;

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении текущего состава
    void currentIndexChanged(const QModelIndex &index);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Mapper для свойств состава
    QDataWidgetMapper* m_mapper;

    //! Хранит индекс предыдущего активного состава
    QPersistentModelIndex  m_oldIndex;
};

#endif // COMPOSITIONPROPWIDGET_H
