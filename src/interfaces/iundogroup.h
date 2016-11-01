#ifndef IUNDOGROUP_H
#define IUNDOGROUP_H

#include <QUndoStack>
#include <QWidget>

//! Класс стека отмена/повтора команд
/*! Класс стека отмена/повтора команд предназначен для ведения истории
 *  выполнения команд с возможностью их отмены или повторения.
*/
class IUndoGroup
{

public:

    //! Добавление стека отмены
    virtual void addStack(QUndoStack *stack) = 0;

    //! Удаление стека отмены
    virtual void removeStack(QUndoStack *stack) = 0;

    //! Добавление QWidget для QUndoStack
    virtual void addWidgetForStack(QUndoStack *stack, QWidget *widget) = 0;

    //! Удаление QWidget для QUndoStack
    virtual void removeWidgetForStack(QWidget *widget) = 0;

};

#define IUndoGroup_iid "com.RTPTechGroup.Raindrop.IUndoGroup"
Q_DECLARE_INTERFACE(IUndoGroup, IUndoGroup_iid)

#endif 
