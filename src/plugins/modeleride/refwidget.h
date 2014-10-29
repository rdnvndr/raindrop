#ifndef REFWIDGET_H
#define REFWIDGET_H

#include "ui_refwidget.h"
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QStringListModel>

//! Диалог редактирования справочника
/*! Диалог предназначен для редактирования справочника
 */

class RefWidget : public QWidget, private Ui::RefWidget
{
    Q_OBJECT

public:
    explicit RefWidget(QWidget *parent = 0);

    //! Деструктор редактора справочника
    virtual ~RefWidget();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Проверка на возможность удаления справочника
    static bool isRemove(const QModelIndex &srcIndex);

    //! Проверка запонено ли имя справочника
    bool isEmpty();

public slots:
    //! Добавление справочника
    void add();

    //! Удаление справочника
    void remove();

    //! Удаление пустой справочника
    void removeEmpty();

    //! Установка текущей справочника
    void setCurrent(const QModelIndex &index);

    //! Перевод справочника в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений справочника
    void submit();

    //! Отмена изменений справочника
    void revert();

    //! Удаление справочника
    void rowsRemoved(const QModelIndex &index,int start,int end);

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении текущей справочника
    void currentIndexChanged(const QModelIndex &index);

    //! Сигнал о редактировании справочника
    void edited(bool flag);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr,
                       const QModelIndex &index, int role = Qt::DisplayRole);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Mapper для свойств справочника
    QDataWidgetMapper* m_mapper;

    //! Хранит индекс предыдущей активной справочника
    int  m_oldIndex;
};

#endif // REFWIDGET_H
