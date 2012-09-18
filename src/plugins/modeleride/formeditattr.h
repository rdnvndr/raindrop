#ifndef FORMEDITATTR_H
#define FORMEDITATTR_H

#include "ui_formeditattr.h"

class FormEditAttr : public QWidget, private Ui::FormEditAttr
{
    Q_OBJECT
    
public:
    explicit FormEditAttr(QWidget *parent = 0);
};

#endif // FORMEDITATTR_H
