#ifndef CLASSTREEVIEW_H
#define CLASSTREEVIEW_H

#include "ui_classtreeview.h"

class ClassTreeView : public QWidget, public Ui::ClassTreeView
{
    Q_OBJECT
    
public:
    explicit ClassTreeView(QWidget *parent = 0);
};

#endif // CLASSTREEVIEW_H
