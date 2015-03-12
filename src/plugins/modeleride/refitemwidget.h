#ifndef REFITEMWIDGET_H
#define REFITEMWIDGET_H

#include "ui_refitemwidget.h"

#include <QStringListModel>
#include <QDataWidgetMapper>

#include <treexmlmodel/modifyproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

#include "refproxymodel.h"
#include "abstractlisteditorwidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования элементов справочника
/*! Диалог предназначен для редактирования элементов справочника
*/

class RefItemWidget : public AbstractListEditorWidget, private Ui::RefItemWidget
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit RefItemWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~RefItemWidget();

    //! Установка модели для редактирования элементов справочника
    void setModel(TreeXmlHashModel *model);

signals:
    //! Сигнал об изменении корневого индекса в прокси  элементов справочника
    void proxyIndexChanged(const QModelIndex &index);

public slots:
    //! Добавление элементов справочника
    void add();

    //! Добавление дочерних элементов справочника
    void addChild();

    //! Перевод в режим редактирования
    void edit(bool flag = true);
};

}}

#endif // REFITEMWIDGET_H
