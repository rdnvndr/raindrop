#ifndef UNDOTEXTDOCUMENT_H
#define UNDOTEXTDOCUMENT_H

#include <QUndoCommand>
#include <QTextDocument>

namespace RTPTechGroup {
namespace SqlEditor {

//! Класс команды стека отмена/повтора команды для текстового документа

class UndoTextDocument : public QUndoCommand
{
public:
    //! Конструктор класса
    explicit UndoTextDocument(QTextDocument *document);

    //! Отмена команды
    virtual void undo();

    //! Повтор команды
    virtual void redo();

private:
    //! Текстовый документ
    QTextDocument *m_document;
};

}}

#endif // UNDOTEXTDOCUMENT_H
