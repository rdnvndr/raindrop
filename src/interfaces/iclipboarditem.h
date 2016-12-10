#ifndef ICLIPBOARDITEM_H
#define ICLIPBOARDITEM_H

#include <iclipboardelement.h>

//! Класс элемента стека буфера обмена
/*! Класс элемента стека буфера обмена предназначен для описания элемента для
 *  которого будет работать буфер обмена
*/
class IClipboardItem: public IClipboardElement {

public:
    //! Конструктор класса
    explicit IClipboardItem(){}

    //! Деструктор класса
    virtual ~IClipboardItem() {}

    //! Возращает возможность вызова команды "Вырезать"
    virtual bool canCut()       = 0;

    //! Возращает возможность вызова команды "Копировать"
    virtual bool canCopy()      = 0;

    //! Возращает возможность вызова команды "Вставить"
    virtual bool canPaste()     = 0;

    //! Возращает возможность вызова команды "Выделить все"
    virtual bool canSelectAll() = 0;

    //! Вызов команды "Вырезать"
    virtual void cut()       = 0;

    //! Вызов команды "Копировать"
    virtual void copy()      = 0;

    //! Вызов команды "Вставить"
    virtual void paste()     = 0;

    //! Вызов команды "Выделить все"
    virtual void selectAll() = 0;


    //! Включение возможности выполнении команды "Вырезать"
    virtual void enableCut(bool enabled) {
        foreach (IClipboardElement *stack, m_elementList)
            stack->enableCut(enabled);
    }

    //! Включение возможности выполнении команды "Копировать"
    virtual void enableCopy(bool enabled) {
        foreach (IClipboardElement *stack, m_elementList)
            stack->enableCopy(enabled);
    }

    //! Включение возможности выполнении команды "Вставить"
    virtual void enablePaste(bool enabled) {
        foreach (IClipboardElement *stack, m_elementList)
            stack->enablePaste(enabled);
    }

    //! Включение возможности выполнении команды "Выделить все"
    virtual void enableSelectAll(bool enabled) {
        foreach (IClipboardElement *stack, m_elementList)
            stack->enableSelectAll(enabled);
    }
};

//Q_DECLARE_INTERFACE(IClipboardItem,"com.RTPTechGroup.Raindrop.IClipboardItem")

#endif 
