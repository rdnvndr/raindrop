#ifndef PROPCOMPOSITION_H
#define PROPCOMPOSITION_H

#include "ui_propcomposition.h"
#include <treexmlmodel/treexmlmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <QDataWidgetMapper>
#include <QStringListModel>

//! Диалог редактирования состава
/*! Диалог предназначен для редактирования состава. Включает в себя диалоги
    редактирования свойств состава и списка атрибутов.
 */

class PropComposition : public QWidget, private Ui::PropComposition
{
    Q_OBJECT
    
public:
    //! Конструктор диалога редактирования состава
    explicit PropComposition(QWidget *parent = 0);

    //! Установка модели структуры классов
    void setModel(TreeXMLModel *model);

    //! Получение модели структуры классов
    TreeXMLModel* model();

public slots:
    //! Установка текущего класса
    void setCurrentClass(QModelIndex index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Закрытие вкладки/подокна
    void closeTab(const QModelIndex &index = QModelIndex());

private:
    //! Получение данных модели
    QVariant modelData(QString typeName, QString attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXMLModel* m_model;
};

#endif // PROPCOMPOSITION_H
