#ifndef ACTIONPROP_H
#define ACTIONPROP_H

#include "ui_actionprop.h"

namespace RTPTechGroup {
namespace MainWindow {

//! Диалоговое окно свойств команд и меню
/*! Диалоговое окно свойств команд и меню предназначено для редактирования
    названия команды и изменения иконки.
*/
class ActionProp : public QDialog, public Ui::ActionProp
{
    Q_OBJECT
    
public:
    //! Конструктор
    explicit ActionProp(QWidget *parent = 0);

};

}
}

#endif // ACTIONPROP_H
