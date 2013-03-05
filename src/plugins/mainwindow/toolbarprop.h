#ifndef TOOLBARPROP_H
#define TOOLBARPROP_H

#include "ui_toolbarprop.h"

class ToolBarProp : public QDialog, private Ui::ToolBarProp
{
    Q_OBJECT
    
public:
    explicit ToolBarProp(QWidget *parent = 0);
    QString text();
    void setText(QString text);
};

#endif // TOOLBARPROP_H
