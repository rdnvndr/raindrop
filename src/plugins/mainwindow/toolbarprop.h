#ifndef TOOLBARPROP_H
#define TOOLBARPROP_H

#include "ui_toolbarprop.h"

//! Диалоговое окно свойств панели инструментов
/*! Диалоговое окно свойств панели инструментов предназначено для редактирования
    названия панели инструментов.
*/

class ToolBarProp : public QDialog, private Ui::ToolBarProp
{
    Q_OBJECT
    
public:
    explicit ToolBarProp(QWidget *parent = 0);

    //! Получить название панели инструментов
    QString text();

    //! Установить название панели инструментов
    void setText(QString text);
};

#endif // TOOLBARPROP_H
