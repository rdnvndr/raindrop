#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include "ui_filterwidget.h"

#include <QDataWidgetMapper>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

#include "abstractitemwidget.h"

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования фильтров класса
/*! Диалог предназначен для редактирования списка фильтров
    класса
*/

class FilterWidget : public AbstractItemWidget, private Ui::FilterWidget
{
    Q_OBJECT
    
public:
    //! Конструктор редактора фильтров
    explicit FilterWidget(QWidget *parent = 0);

    //! Деструктор редактора фильтров
    virtual ~FilterWidget();

    //! Установка модели для редактирования составов класса
    void setModel(TreeXmlHashModel *model);

public slots:
    //! Добавление фильтра класса
    void add();

    //! Вызов редактирования фильтра класса
    void edit(bool flag = true);

    //! Проверка находится ли в режиме редактирования
    bool isEdit();

    //! Установка отображения родительских фильтров класса
    void showParent(bool flag);

signals:
    //! Сигнал о добавлении данных
    void dataAdded(const QModelIndex &index);

    //! Сигнал о вызове редактирования данных
    void dataEdited(const QModelIndex &index);
};

}}

#endif // FILTERWIDGET_H
