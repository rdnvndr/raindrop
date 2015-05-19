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
    void addClipboardItem(IClipboardItem *item);

    //! Удаление компонента для буфера обмена
    void removeClipboardItem(IClipboardItem *item);

};

Q_DECLARE_INTERFACE(IClipboardStack,"com.RTPTechGroup.Raindrop.IClipboardStack/1.0")

#endif 
