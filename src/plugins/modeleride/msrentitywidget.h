#ifndef MSRENTITYWIDGET_H
#define MSRENTITYWIDGET_H

#include "ui_msrentitywidget.h"

class MsrEntityWidget : public QWidget, private Ui::MsrEntityWidget
{
    Q_OBJECT

public:
    explicit MsrEntityWidget(QWidget *parent = 0);
};

#endif // MSRENTITYWIDGET_H
