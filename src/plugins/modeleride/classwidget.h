#ifndef CLASSWIDGET_H
#define CLASSWIDGET_H

#include "ui_classwidget.h"
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlmodel.h>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QStringListModel>

//! Диалог редактирования класса
/*! Диалог предназначен для класса
 */

class ClassWidget : public QWidget, private Ui::ClassWidget
{
    Q_OBJECT
    
public:
    //! Конструктор редактора класса
    explicit ClassWidget(QWidget *parent = 0);

    //! Деструктор редактора класса
    virtual ~ClassWidget();

    //! Установка модели структуры классов
    void setModel(TreeXMLModel *model);

    //! Проверка на возможность удаления класса
    bool isRemove(QModelIndex srcIndex);

public slots:
    //! Добавление класса
    void add();

    //! Удаление класса
    void remove();

    //! Удаление пустого класса
    void removeEmpty();

    //! Установка текущего класса
    void setCurrent(QModelIndex index);

    //! Перевод класса в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений класса
    void submit();

    //! Отмена изменений класса
    void revert();

    //! Удаление классов
    void rowsRemoved(QModelIndex index,int start,int end);

signals:
    //! Сигнал об изменении данных
    void dataChanged(QModelIndex index);

    //! Сигнал об удалении данных
    void dataRemoved(QModelIndex index);

    //! Сигнал об изменении текущего класса
    void currentIndexChanged(QModelIndex index);

private:
     //! Получение данных модели
    QVariant modelData(QString typeName, QString attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXMLModel* m_model;

     //! Mapper для свойств класса
    QDataWidgetMapper* m_mapper;

    //! Список типов класса
    QStringListModel *m_typeClassModel;

    //! Хранит индекс предыдущего активного класса
    int  m_oldIndex;
};

#endif // CLASSWIDGET_H
