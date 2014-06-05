#ifndef MSRUNITWIDGET_H
#define MSRUNITWIDGET_H

#include "ui_msrunitwidget.h"

class MsrUnitWidget : public QWidget, private Ui::MsrUnitWidget
{
    Q_OBJECT

public:
    explicit MsrUnitWidget(QWidget *parent = 0);
};

#endif // MSRUNITWIDGET_H
