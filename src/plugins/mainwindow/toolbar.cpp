#include <QDropEvent>
#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QToolButton>
#include <QDebug>

#include "toolbar.h"
#include "menu.h"
#include "mimedataobject.h"
#include "actionprop.h"

ToolBar::ToolBar(QWidget *parent) :
    QToolBar(parent)
{
    setAcceptDrops(true);
    setIconSize(QSize(20,20));

    m_dragPos = QPoint(-1,-1);
    m_activeAction = NULL;
}

ToolBar::~ToolBar()
{

}

void ToolBar::mouseMoveEvent(QMouseEvent *event)
{
    QToolBar::mouseMoveEvent(event);
    QAction *action  = this->actionAt(m_dragPos);

    if (event->buttons() & Qt::LeftButton && action) {
        int distance = (event->pos() - m_dragPos).manhattanLength();
        if (distance > QApplication::startDragDistance() ) {

            QDrag *drag = new QDrag(this);
            MimeDataObject *mimeData = new MimeDataObject();

            mimeData->setObject(action);
            drag->setMimeData(mimeData);

            if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
                if (drag->target() != this || action->isSeparator())
                    this->removeAction(action);
            return;
        }
    }
}

void ToolBar::mousePressEvent(QMouseEvent *event)
{
    QToolBar::mousePressEvent(event);
}

void ToolBar::dropEvent(QDropEvent *event)
{
    const MimeDataObject *mimeData = qobject_cast<const MimeDataObject *>(event->mimeData());
    QAction *aAction = qobject_cast<QAction *>(mimeData->object());

    if (aAction) {

        if (aAction->menu())
            if (aAction->objectName() == "actionNewMenu") {
                aAction = (new Menu(aAction->text()))->menuAction();
            }

        QAction* eAction = this->actionAt(event->pos());
        QRect rect = actionGeometry(eAction);
        if (this->orientation() == Qt::Horizontal)
            eAction = this->actionAt(QPoint(event->pos().x()+rect.width()/2,
                                            event->pos().y()));
        else
            eAction = this->actionAt(QPoint(event->pos().x(),
                                            event->pos().y()+rect.height()/2));

        if (eAction) {
            if (aAction->isSeparator())
                insertSeparator(eAction);
            else
                insertAction(eAction,aAction);
        } else {
            if (aAction->isSeparator())
                addSeparator();
            else
                addAction(aAction);
        }

        event->acceptProposedAction();
    }
}

void ToolBar::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qobject")) {
        event->acceptProposedAction();
    }
    m_dragPos = QPoint(-1,-1);
}

void ToolBar::dragMoveEvent(QDragMoveEvent *event)
{
    const MimeDataObject *mimeData
            = qobject_cast<const MimeDataObject *>(event->mimeData());

    QAction* eAction = this->actionAt(event->pos());
    if (mimeData->hasFormat("application/x-qobject"))
        if (mimeData->object() != eAction && eAction)
            if (eAction->menu() && !eAction->menu()->isVisible()) {
                if (m_activeAction)
                    m_activeAction->menu()->close();
                QToolButton *toolButton = qobject_cast<QToolButton *>(widgetForAction(eAction));
                QPoint point = QPoint(toolButton->x(), toolButton->y()+toolButton->height());
                eAction->menu()->popup(mapToGlobal(point));
                m_activeAction = eAction;
            }

    event->accept();
}

bool ToolBar::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object)

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (event->type() == QEvent::MouseMove)
        this->mouseMoveEvent(mouseEvent);

    if (event->type() == QEvent::MouseButtonPress)
        if (mouseEvent->button() == Qt::LeftButton)
            m_dragPos = mapFromGlobal(mouseEvent->globalPos());

    return false;
}

void ToolBar::actionEvent(QActionEvent *event)
{
    QToolBar::actionEvent(event);
    if (event->type() == QEvent::ActionAdded)
        if (event->action()) {
            QWidget *widget = widgetForAction(event->action());
            if (widget)
                widget->installEventFilter(this);
        }
}

void ToolBar::contextMenuEvent(QContextMenuEvent *event)
{
    if (this->actionAt(event->pos())!=NULL) {
        // Создание контекстного меню
        QMenu *contextMenu = new QMenu();
        QAction *action = new QAction(tr("Удалить"),this);
        connect(action,SIGNAL(triggered()), this,SLOT(removeContextAction()));
        contextMenu->addAction(action);
        contextMenu->addSeparator();
        action = new QAction(tr("Свойства..."),this);
        connect(action,SIGNAL(triggered()), this, SLOT(showActionProp()));
        contextMenu->addAction(action);
        m_contextAction = this->actionAt(event->pos());
        contextMenu->exec(event->globalPos());
        delete contextMenu;
    } else
        QToolBar::contextMenuEvent(event);
}

void ToolBar::removeContextAction()
{
    removeAction(m_contextAction);
}

void ToolBar::showActionProp()
{
    ActionProp *actionProp = new ActionProp();
    actionProp->lineEditName->setText(m_contextAction->text());
    if (actionProp->exec() == QDialog::Accepted) {
        m_contextAction->setText(actionProp->lineEditName->text());
    }
}
