#ifndef ATTRWIDGET_H
#define ATTRWIDGET_H

#include "ui_attrwidget.h"
#include <QDataWidgetMapper>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlmodel.h>

//! Диалог редактирования атрибутов класса
/*! Диалог предназначен для редактирования атрибутов класса
    и состава
 */

class AttrWidget : public QWidget, private Ui::AttrWidget
{
    Q_OBJECT
    
public:
    //! Конструктор класса
    explicit AttrWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~AttrWidget();

    //! Установка модели для редактирования атрибутов класса
    void setModel(TreeXMLModel *model);

public slots:
    //! Добавление атрибута класса
    void add();

    //! Удаление атрибута класса
    void remove();

    //! Установка текущего атрибута класса
    void setCurrent(QModelIndex index);

    //! Применение изменений атрибута класса
    void submit();

    //! Перевод в режим редактирования атрибута класса
    void edit(bool flag = true);

    //! Отмена изменений атрибута класса
    void revert();

    //! Установка отображения родительских атрибутов класса
    void showParentAttr(bool flag);

    //! Обработка изменение типа атрибута
    void changeType(QString s);

    //! Установка родителя атрибутов
    void setRootIndex(QModelIndex index);

signals:
    //! Сигнал об изменении данных
    void dataChanged(QModelIndex index);

    //! Сигнал об удалении данных
    void dataRemoved(QModelIndex index);

    //! Сигнал об изменении текущего атрибута
    void currentIndexChanged(QModelIndex index);

private:
    //! Получение данных модели
    QVariant modelData(QString typeName, QString attr, const QModelIndex &index);

    //! Прокси модель для атрибутов класса
    TableXMLProxyModel* m_attrModel;

    //! Модель структуры классов
    TreeXMLModel* m_model;

    //! Mapper для свойств атрибутов
    QDataWidgetMapper* m_mapperAttr;

    //! Список типов атрибута
    QStringListModel *m_typeAttrModel;
};

#endif // ATTRWIDGET_H
