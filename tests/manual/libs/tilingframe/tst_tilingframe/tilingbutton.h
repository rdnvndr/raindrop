#ifndef TILINGBUTTON_H
#define TILINGBUTTON_H

#include <QWidget>
#include <QPushButton>

#include <tilingframe/tilingwidget.h>
using namespace RTPTechGroup::Widgets;

class TilingButton : public TilingWidget
{
    Q_OBJECT

public:
    explicit TilingButton(QWidget *parent = 0);
    TilingWidget *clone();
};

#endif // TILINGBUTTON_H
