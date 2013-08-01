#ifndef PROPCOMPOSITION_H
#define PROPCOMPOSITION_H

#include "ui_propcomposition.h"
#include <treexmlmodel/treexmlhashmodel.h>
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
    void setModel(TreeXmlHashModel *model);

    //! Получение модели структуры классов
    TreeXmlHashModel* model();

public slots:
    //! Установка текущего класса
    void setCurrentClass(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Закрытие вкладки/подокна
    void closeTab(const QModelIndex &index = QModelIndex());

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;
};

#endif // PROPCOMPOSITION_H
