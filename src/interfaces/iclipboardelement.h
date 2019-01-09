#ifndef ICLIPBOARDELEMENT_H
#define ICLIPBOARDELEMENT_H

#include <QList>

//! Класс абстрактного элемента буфера обмена
/*! Класс абстрактного элемента буфера обмена предназначен для описания
 *  буфера обмена
*/
class IClipboardElement {

public:
    //! Конструктор класса
    explicit IClipboardElement(){}

    //! Деструктор класса
    virtual ~IClipboardElement() {
        for (IClipboardElement *item : qAsConst(m_elementList))
            item->m_elementList.removeAll(this);
        m_elementList.clear();
    }

    //! Включение возможности выполнении команды "Вырезать"
    virtual void enableCut(bool enabled) = 0;

    //! Включение возможности выполнении команды "Копировать"
    virtual void enableCopy(bool enabled) = 0;

    //! Включение возможности выполнении команды "Вставить"
    virtual void enablePaste(bool enabled) = 0;

    //! Включение возможности выполнении команды "Выделить все"
    virtual void enableSelectAll(bool enabled) = 0;

protected:
    //! Добавление элемента
    void addClipboardElement(IClipboardElement *element) {
        m_elementList.append(element);
        element->m_elementList.append(this);
    }

    //! Удаление элемента
    void removeClipboardElement(IClipboardElement *element) {
       element->m_elementList.removeAll(this);
       m_elementList.removeAll(element);
    }

    //! Список элементов
    QList<IClipboardElement *> m_elementList;
};

#endif 
