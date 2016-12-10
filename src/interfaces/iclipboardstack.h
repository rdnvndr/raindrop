#ifndef ICLIPBOARDSTACK_H
#define ICLIPBOARDSTACK_H

#include <iclipboardelement.h>
#include <iclipboarditem.h>
class IClipboardItem;

//! Класс стека буфера обмена
/*! Класс стека буфера обмена предназначен для работы с буфером обмена
 *  Вырезанием, Копированием, Вставкой и Выбором всего
*/
class IClipboardStack: public IClipboardElement {

public:
    //! Конструктор класса
    explicit IClipboardStack(){}

    //! Деструктор класса
    virtual ~IClipboardStack(){}

    //! Добавление компонента для буфера обмена
    virtual void addClipboardItem(IClipboardItem *item) {
        this->addClipboardElement(item);
    }

    //! Удаление компонента для буфера обмена
    virtual void removeClipboardItem(IClipboardItem *item) {
       this->removeClipboardElement(item);
    }
};

#define IClipboardStack_iid "com.RTPTechGroup.Raindrop.IClipboardStack"
Q_DECLARE_INTERFACE(IClipboardStack, IClipboardStack_iid)

#endif
