#ifndef COMPOSITIONWIDGET_H
#define COMPOSITIONWIDGET_H

#include "ui_compositionwidget.h"

#include <QDataWidgetMapper>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

#include "abstractitemwidget.h"

//! Диалог редактирования составов класса
/*! Диалог предназначен для редактирования списка составов
    класса
*/


namespace RTPTechGroup {
namespace ModelerIde {

class CompositionWidget : public AbstractItemWidget, private Ui::CompositionWidget
{
    Q_OBJECT
    
public:
    //! Конструктор редактора составов класса
    explicit CompositionWidget(QWidget *parent = 0);

    //! Деструктор редактора составов класса
    virtual ~CompositionWidget();

      //! Установка модели для редактирования составов класса
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

public slots:
    //! Добавление состава класса
    bool add();

    //! Применение изменений состава класса
    void submit();

    //! Проверка находится ли в режиме редактирования
    bool isEdit();

    //! Перевод в режим редактирования состава класса
    void edit(bool flag);

    //! Установка отображения родительских составов класса
    void showParent(bool flag);
};

}}

#endif // COMPOSITIONWIDGET_H
