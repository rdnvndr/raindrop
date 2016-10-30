#ifndef ICLIPBOARDSTACK_H
#define ICLIPBOARDSTACK_H

#include <iclipboarditem.h>

//! Класс стека буфера обмена
/*! Класс стека буфера обмена предназначен для работы с буфером обмена
 *  Вырезанием, Копированием, Вставкой и Выбором всего
*/
class IClipboardStack {

public:

    //! Добавление компонента для буфера обмена
    virtual void addClipboardItem(IClipboardItem *item) = 0;

    //! Удаление компонента для буфера обмена
    virtual void removeClipboardItem(IClipboardItem *item) = 0;

};

#define IClipboardStack_iid "com.RTPTechGroup.Raindrop.IClipboardStack"
Q_DECLARE_INTERFACE(IClipboardStack, IClipboardStack_iid)

#endif
