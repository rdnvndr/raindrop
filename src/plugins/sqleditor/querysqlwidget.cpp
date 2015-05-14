#include "querysqlwidget.h"

#include <QtGui>
#include <plugin/iplugin.h>

using namespace RTPTechGroup::Plugin;

namespace RTPTechGroup {
namespace SqlEditor {

class UndoTextDocument : public QUndoCommand
{
public:
    explicit UndoTextDocument(QTextDocument *document)
        : QUndoCommand()
    {
        m_document = document;
        setText(QObject::tr("Изменение запроса"));
    }
    virtual void undo() { m_document->undo();}
    virtual void redo() { m_document->redo();}

private:
    QTextDocument *m_document;
};

QuerySqlWidget::QuerySqlWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_undoStack = new QUndoStack(this);
    PluginManager* pluginManager = PluginManager::instance();
    m_undoGroup = qobject_cast<IUndoGroup*>(
                pluginManager->interfaceObject("IUndoGroup"));
    m_undoGroup->addStack(m_undoStack);
    m_undoGroup->addWidgetForStack(m_undoStack, this->toolButtonRun);
    m_undoGroup->addWidgetForStack(m_undoStack, this);
    plainQueryEdit->installEventFilter(this);
    connect(plainQueryEdit->document(), SIGNAL(undoCommandAdded()),
            this, SLOT(undoCommandAdd()));

    m_model = new QSqlQueryModel();

    tableView->setModel(m_model);
    m_sqlHighlighter = new SqlHighlighter(plainQueryEdit->document());

    connect(toolButtonRun,SIGNAL(clicked()),this,SLOT(runQuery()));
}

QuerySqlWidget::~QuerySqlWidget()
{
    delete m_sqlHighlighter;
    delete m_model;
    delete m_undoStack;
}

bool QuerySqlWidget::eventFilter(QObject *target, QEvent *event)
{
    if (target == plainQueryEdit) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = (QKeyEvent *)event;
            if ((keyEvent->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)
                    || keyEvent->modifiers() == Qt::ControlModifier)
                    && keyEvent->key() == Qt::Key_Z)
            {
                QApplication::sendEvent(this, event);
                return true;
            }
        }
    }
    return QWidget::eventFilter(target, event);
}

void QuerySqlWidget::runQuery() {
    m_model->setQuery(plainQueryEdit->toPlainText());
    if (m_model->lastError().isValid()){
        plainLogEdit->setPlainText(m_model->lastError().text());
        tabWidget->setCurrentIndex(1);
    }
    else{

        if (m_model->rowCount()<1)
            tabWidget->setCurrentIndex(1);
        else{
            tableView->show();
            tabWidget->setCurrentIndex(0);
        }
        if (m_model->lastError().text().count())
            plainLogEdit->setPlainText(tr("Запрос успешно выполнен"));
        else
            plainLogEdit->setPlainText(m_model->lastError().text());
    }
}

void QuerySqlWidget::undoCommandAdd()
{
    UndoTextDocument *undoTextDocument
            = new UndoTextDocument(plainQueryEdit->document());
    m_undoStack->push(undoTextDocument);
}

}}
