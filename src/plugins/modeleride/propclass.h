#ifndef PROPCLASS_H
#define PROPCLASS_H

#include "ui_propclass.h"
#include <treexmlmodel/treexmlmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <QDataWidgetMapper>
#include <QStringListModel>

//! Диалог редактирования класса
/*! Диалог предназначен для редактирования класса. Включает в себя диалоги
    редактирования свойств класса, списка атрибутов и списка составов
 */

class PropClass : public QWidget, public Ui::PropClass
{
    Q_OBJECT
    
public:
    //! Конструктор диалога редактирования классов
    explicit PropClass(QWidget *parent = 0);

    //! Деструктор диалога редактирования классов
    virtual ~PropClass();

    //! Установка модели структуры классов
    void setModel(TreeXMLModel *model);

    //! Получение модели структуры классов
    TreeXMLModel* model();

signals:
    //! Сигнал вызова редактирования состава
    void editComposition(QModelIndex index);

public slots:
    //! Установка текущего класса
    void setCurrentClass(QModelIndex index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Закрытие вкладки/подокна
    void closeTab(const QModelIndex &index);

    //! Вызов редактирования состава
    void onEditComposition(QModelIndex index);
private:
    //! Получение данных модели
    QVariant modelData(QString typeName, QString attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXMLModel* m_model;
};

#endif // PROPCLASS_H
