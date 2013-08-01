#ifndef ATTRWIDGET_H
#define ATTRWIDGET_H

#include <QStringListModel>
#include <QDataWidgetMapper>

#include "ui_attrwidget.h"
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>


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
    void setModel(TreeXmlHashModel *model);

public slots:
    //! Добавление атрибута класса
    void add();

    //! Удаление атрибута класса
    void remove();

    //! Установка текущего атрибута класса
    void setCurrent(const QModelIndex &index);

    //! Применение изменений атрибута класса
    void submit();

    //! Перевод в режим редактирования атрибута класса
    void edit(bool flag = true);

    //! Отмена изменений атрибута класса
    void revert();

    //! Установка отображения родительских атрибутов класса
    void showParentAttr(bool flag);

    //! Обработка изменения типа атрибута
    void changeType(const QString &typeName);

    //! Установка родителя атрибутов
    void setRootIndex(const QModelIndex &index);

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении текущего атрибута
    void currentIndexChanged(const QModelIndex &index);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr,
                       const QModelIndex &index);

    //! Прокси модель для атрибутов класса
    TableXMLProxyModel* m_attrModel;

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Mapper для свойств атрибутов
    QDataWidgetMapper* m_mapperAttr;

    //! Список типов атрибута
    QStringListModel *m_typeAttrModel;
};

#endif // ATTRWIDGET_H
