#ifndef PROPENTITY_H
#define PROPENTITY_H

#include "ui_propentity.h"

class PropEntity : public QWidget, private Ui::PropEntity
{
    Q_OBJECT

public:
    explicit PropEntity(QWidget *parent = 0);
};

#endif // PROPENTITY_H
