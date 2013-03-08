#ifndef ACTIONPROP_H
#define ACTIONPROP_H

#include "ui_actionprop.h"

class ActionProp : public QDialog, public Ui::ActionProp
{
    Q_OBJECT
    
public:
    explicit ActionProp(QWidget *parent = 0);

};

#endif // ACTIONPROP_H
