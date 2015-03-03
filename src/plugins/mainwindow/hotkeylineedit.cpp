#include <QKeyEvent>
#include <QDebug>
#include "hotkeylineedit.h"

namespace RTPTechGroup {
namespace MainWindow {

HotKeyLineEdit::HotKeyLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    connect(this, SIGNAL(textChanged(const QString&)),
            this, SLOT(onTextChanged(const QString&)));
}

const QKeySequence &HotKeyLineEdit::keySequence() const
{
    return m_keySequence;
}

void HotKeyLineEdit::setKeySequence(const QKeySequence &keySequence)
{
    m_keySequence = keySequence;
    m_oldKeySequence = keySequence;
    setText(m_keySequence.toString());
}

void HotKeyLineEdit::resetKeySequence()
{
    m_keySequence = m_oldKeySequence;
    setText(m_keySequence.toString());
}

void HotKeyLineEdit::keyPressEvent(QKeyEvent *event)
{
    m_keySequence = QKeySequence();
    int key = event->key();

    if( Qt::Key_Shift   == key ||
            Qt::Key_Control == key ||
            Qt::Key_Meta    == key ||
            Qt::Key_Alt     == key )
    {
        key = 0;
    }

    m_keySequence = QKeySequence (
                (0 != (event->modifiers() & Qt::MetaModifier)    ? Qt::META  : Qt::UNICODE_ACCEL) +
                (0 != (event->modifiers() & Qt::ShiftModifier)   ? Qt::SHIFT : Qt::UNICODE_ACCEL) +
                (0 != (event->modifiers() & Qt::ControlModifier) ? Qt::CTRL  : Qt::UNICODE_ACCEL) +
                (0 != (event->modifiers() & Qt::AltModifier)     ? Qt::ALT   : Qt::UNICODE_ACCEL) +
                key
                );

    event->ignore();
    setText(m_keySequence.toString());

    if (key != 0)
        emit changeHotKey(m_keySequence);
}

void HotKeyLineEdit::showEvent(QShowEvent *event)
{
    setText(m_keySequence.toString());
    QLineEdit::showEvent(event);
}

void HotKeyLineEdit::onTextChanged(const QString &)
{
    if( text() != m_keySequence.toString() ) {
        setText(m_keySequence.toString());
    }
}

}
}
