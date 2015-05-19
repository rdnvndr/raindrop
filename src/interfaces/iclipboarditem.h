#ifndef ICLIPBOARDITEM_H
#define ICLIPBOARDITEM_H

#include <QWidget>

//! Класс элемента стека буфера обмена
/*! Класс элемента стека буфера обмена предназначен для описания элемента для
 *  которого будет работать буфер обмена
*/
class IClipboardItem {

public:
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

};

Q_DECLARE_INTERFACE(IClipboardItem,"com.RTPTechGroup.Raindrop.IClipboardItem/1.0")

#endif 
