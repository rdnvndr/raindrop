#ifndef TEXTCLIPBOARDITEM_H
#define TEXTCLIPBOARDITEM_H

#include <QPlainTextEdit>

#include <iclipboarditem.h>

namespace RTPTechGroup {
namespace SqlEditor {

//! Класс элемента стека буфера обмена редактора sql запросов

class TextClipboardItem: public QObject, public IClipboardItem
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

signals:
    //! Сигнал о возможности выполнении команды "Вырезать"
    void canCutChanged(bool canCut);

    //! Сигнал о возможности выполнении команды "Копировать"
    void canCopyChanged(bool canCopy);

    //! Сигнал о возможности выполнении команды "Вставить"
    void canPasteChanged(bool canPaste);

    //! Сигнал о возможности выполнении команды "Выделить все"
    void canSelectAllChanged(bool canSelectAll);
private slots:
    //! Обработка сигнала выбора текста
    void selectionChange();

private:
    QPlainTextEdit *m_editor;

};

}}

#endif // TEXTCLIPBOARDITEM_H
