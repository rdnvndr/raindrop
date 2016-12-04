#include "textclipboarditem.h"

#include <QTextFrame>
#include <QApplication>
#include <QClipboard>

namespace RTPTechGroup {
namespace SqlEditor {

TextClipboardItem::TextClipboardItem(QPlainTextEdit *editor)
    : QObject(editor), m_editor(editor)
{
    connect(editor, &QPlainTextEdit::selectionChanged,
            this, &TextClipboardItem::selectionChange);
}

bool TextClipboardItem::canCut()
{
    if (qApp->focusWidget() == m_editor) {
        QTextCursor cursor = m_editor->textCursor();
        return (cursor.hasSelection());
    }
    return false;
}

bool TextClipboardItem::canCopy()
{
    if (qApp->focusWidget() == m_editor) {
        QTextCursor cursor = m_editor->textCursor();
        return (cursor.hasSelection());
    }
    return false;
}

bool TextClipboardItem::canPaste()
{
    if (qApp->focusWidget() == m_editor) {
        QClipboard *clipboard = QApplication::clipboard();
        return !clipboard->text().isEmpty();
    }
    return false;
}

bool TextClipboardItem::canSelectAll()
{
    return (qApp->focusWidget() == m_editor);
}

void TextClipboardItem::cut()
{
    QTextCursor cursor = m_editor->textCursor();
    if (cursor.hasSelection()) {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(cursor.selectedText());
        cursor.removeSelectedText();
    }
}

void TextClipboardItem::copy()
{
    QTextCursor cursor = m_editor->textCursor();
    if (cursor.hasSelection()) {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(cursor.selectedText());
        cursor.clearSelection();
    }
}

void TextClipboardItem::paste()
{
    QTextCursor cursor = m_editor->textCursor();
    QClipboard *clipboard = QApplication::clipboard();
    cursor.insertText(clipboard->text());
}

void TextClipboardItem::selectAll()
{
    QTextCursor cursor = m_editor->textCursor();
    QTextFrame *frame = cursor.currentFrame();

    cursor.beginEditBlock();
    cursor.setPosition(frame->firstPosition());
    cursor.setPosition(frame->lastPosition(), QTextCursor::KeepAnchor);
    cursor.endEditBlock();

    m_editor->setTextCursor(cursor);
}

void TextClipboardItem::selectionChange()
{
    bool hasSelection = m_editor->textCursor().hasSelection();
    emit canCutChanged(hasSelection);
    emit canCopyChanged(hasSelection);
}

}}
