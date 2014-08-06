#ifndef LOVWIDGET_H
#define LOVWIDGET_H

#include "ui_lovwidget.h"
#include "treexmlmodel/treexmlhashmodel.h"

class LovWidget : public QWidget, private Ui::LovWidget
{
    Q_OBJECT

public:
    explicit LovWidget(QWidget *parent = 0);

    //! Проверка на возможность удаления списка значений
    static bool isRemove(const QModelIndex &srcIndex);

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

public slots:

    //! Установка текущего списка значений
    void setCurrentLov(const QModelIndex &index);
};

#endif // LOVWIDGET_H
