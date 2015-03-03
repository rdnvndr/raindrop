#ifndef TREEFILTERWIDGET_H
#define	TREEFILTERWIDGET_H

#include  "ui_treefilterwidget.h"

namespace RTPTechGroup {
namespace TreeDockWidget {

//! Класс визуального компонента дерева с фильтров
class TreeFilterWidget: public QWidget, public Ui::TreeFilterWidget{
        Q_OBJECT
public:
    //! Конструктор визуального компонента дерева с фильтров
    TreeFilterWidget(QWidget *pwgt = 0);

};

}}

#endif
