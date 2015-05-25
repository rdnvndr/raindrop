#include "undotextdocument.h"

namespace RTPTechGroup {
namespace SqlEditor {

UndoTextDocument::UndoTextDocument(QTextDocument *document)
    : QUndoCommand()
{
    m_document = document;
    setText(QObject::tr("Изменение запроса"));
}

void UndoTextDocument::undo()
{
    m_document->undo();
}

void UndoTextDocument::redo()
{
    m_document->redo();
}

}}
