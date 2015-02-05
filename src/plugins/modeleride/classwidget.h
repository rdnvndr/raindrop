#ifndef CLASSWIDGET_H
#define CLASSWIDGET_H

#include "ui_classwidget.h"
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QStringListModel>

//! Диалог редактирования класса
/*! Диалог предназначен для редактирования класса
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
    void setModel(TreeXmlHashModel *model);

    //! Проверка на возможность удаления класса
    static bool isRemove(QModelIndex srcIndex);

public slots:
    //! Добавление класса
    void add();

    //! Удаление класса
    void remove();

    //! Удаление пустого класса
    bool removeEmpty();

    //! Установка текущего класса
    void setCurrent(const QModelIndex &index);

    //! Возращает находится ли в режиме редактирования
    bool isEdit();

    //! Перевод класса в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений класса
    void submit();

    //! Отмена изменений класса
    void revert();

    //! Удаление классов
    void rowsRemoved(const QModelIndex &index,int start,int end);

    //! Сообщение о неверном имени класса
    void validateClassName(QValidator::State state) const;

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении текущего класса
    void currentIndexChanged(const QModelIndex &index);

private:

    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

     //! Mapper для свойств класса
    QDataWidgetMapper* m_mapper;

    //! Список типов класса
    QStringListModel *m_typeClassModel;

    //! Хранит индекс предыдущего активного класса
    int  m_oldIndex;
};

#endif // CLASSWIDGET_H
