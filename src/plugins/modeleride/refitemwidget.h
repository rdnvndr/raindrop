#ifndef REFITEMWIDGET_H
#define REFITEMWIDGET_H

#include "ui_refitemwidget.h"

class RefItemWidget : public QWidget, private Ui::RefItemWidget
{
    Q_OBJECT

public:
    explicit RefItemWidget(QWidget *parent = 0);
};

#endif // REFITEMWIDGET_H
