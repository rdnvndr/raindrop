#ifndef LOVVALUEWIDGET_H
#define LOVVALUEWIDGET_H

#include "ui_lovvaluewidget.h"
#include <treexmlmodel/treexmlhashmodel.h>

class LovValueWidget : public QWidget, private Ui::LovValueWidget
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit LovValueWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~LovValueWidget();

    //! Установка модели для редактирования значений списка
    void setModel(TreeXmlHashModel *model);

};

#endif // LOVVALUEWIDGET_H
