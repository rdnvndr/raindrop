#ifndef CLASSTREEVIEW_H
#define CLASSTREEVIEW_H

#include "ui_classtreeview.h"

//! Класс для отображения дерева классов

class ClassTreeView : public QWidget, public Ui::ClassTreeView
{
    Q_OBJECT
    
public:
    //! Конструктор дерева классов
    explicit ClassTreeView(QWidget *parent = 0);
};

#endif // CLASSTREEVIEW_H
