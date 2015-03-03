#ifndef PROPCLASS_H
#define PROPCLASS_H

#include "ui_propclass.h"
#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <QDataWidgetMapper>
#include <QStringListModel>

namespace RTPTechGroup {
namespace ModelerIde {

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
    void setModel(TreeXmlHashModel *model);

    //! Получение модели структуры классов
    TreeXmlHashModel* model();

signals:
    //! Сигнал вызова редактирования состава
    void editComposition(const QModelIndex &index);

    //! Сигнал вызова редактирования фильтра
    void editFilter(const QModelIndex &index);

public slots:
    //! Установка текущего класса
    void setCurrentClass(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Закрытие вкладки/подокна
    void closeTab(const QModelIndex &index);

    //! Вызов редактирования состава
    void onEditComposition(const QModelIndex &index);

    //! Вызов редактирования фильтра
    void onEditFilter(const QModelIndex &index);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;
};

}}

#endif // PROPCLASS_H
