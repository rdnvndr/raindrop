#ifndef TEXTCLIPBOARDITEM_H
#define TEXTCLIPBOARDITEM_H

#include <QPlainTextEdit>

#include <iclipboarditem.h>

namespace RTPTechGroup {
namespace SqlEditor {

//! Класс элемента стека буфера обмена редактора sql запросов

class TextClipboardItem: public IClipboardItem
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit TextClipboardItem(QPlainTextEdit *editor);

    //! Возращает возможность вызова команды "Вырезать"
    bool canCut();

    //! Возращает возможность вызова команды "Копировать"
    bool canCopy();

    //! Возращает возможность вызова команды "Вставить"
    bool canPaste();

    //! Возращает возможность вызова команды "Выделить все"
    bool canSelectAll();

    //! Вызов команды "Вырезать"
    void cut();

    //! Вызов команды "Копировать"
    void copy();

    //! Вызов команды "Вставить"
    void paste();

    //! Вызов команды "Выделить все"
    void selectAll();

private:
    QPlainTextEdit *m_editor;

};

}}

#endif // TEXTCLIPBOARDITEM_H
