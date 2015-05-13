#ifndef IUNDOGROUP_H
#define IUNDOGROUP_H

#include <QUndoGroup>

//! Класс стека отмена/повтора команд
/*! Класс стека отмена/повтора команд предназначен для ведения истории
 *  выполнения команд с возможностью их отмены или повторения.
*/
class IUndoGroup: public QUndoGroup
{

public:
    //! Конструктор
    explicit IUndoGroup(QObject* parent) : QUndoGroup(parent){};

public slots:
    //! Добавление QWidget для QUndoStack
    virtual void addWidgetForStack(QUndoStack *stack, QWidget *widget) = 0;

    //! Удаление QWidget для QUndoStack
    virtual void removeWidgetForStack(QWidget *widget) = 0;

};

Q_DECLARE_INTERFACE(IUndoGroup,"com.RTPTechGroup.Raindrop.IUndoGroup/1.0")

#endif 
