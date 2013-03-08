#include <QDropEvent>
#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QToolButton>
#include <QDrag>
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
    setContextMenuPolicy(Qt::NoContextMenu);

    m_edited = false;
}

ToolBar::~ToolBar()
{

}

void ToolBar::mouseMoveEvent(QMouseEvent *event)
{
    QToolBar::mouseMoveEvent(event);
    QAction *action  = this->actionAt(m_dragPos);

    if (event->buttons() & Qt::LeftButton && action && isEdited()) {
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

    if (aAction && isEdited()) {

        if (aAction->menu())
            if (aAction->objectName() == "actionNewMenu") {
                Menu *menu =  new Menu(aAction->text());
                menu->setEdited(true);
                menu->setIcon(aAction->icon());
                aAction = menu->menuAction();
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
    if (event->mimeData()->hasFormat("application/x-qobject")
            && isEdited()) {
        event->acceptProposedAction();
    }
    m_dragPos = QPoint(-1,-1);
}

void ToolBar::dragMoveEvent(QDragMoveEvent *event)
{
    const MimeDataObject *mimeData
            = qobject_cast<const MimeDataObject *>(event->mimeData());

    QAction* eAction = this->actionAt(event->pos());
    if (mimeData->hasFormat("application/x-qobject") && isEdited())
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

    if (event->type() == QEvent::MouseButtonPress && isEdited()) {
        if (mouseEvent->button() == Qt::RightButton) {
            m_contextAction = this->actionAt(
                        mapFromGlobal(mouseEvent->globalPos()));
            if (m_contextAction) {
                // Создание контекстного меню
                QMenu *contextMenu = new QMenu();
                QAction *action = new QAction(tr("Удалить"),this);
                connect(action,SIGNAL(triggered()), this,SLOT(removeContextAction()));
                contextMenu->addAction(action);
                contextMenu->addSeparator();
                action = new QAction(tr("Свойства..."),this);
                connect(action,SIGNAL(triggered()), this, SLOT(showActionProp()));
                contextMenu->addAction(action);
                contextMenu->exec(mouseEvent->globalPos());
                delete contextMenu;
            }
        }

        if (mouseEvent->button() == Qt::LeftButton)
            m_dragPos = mapFromGlobal(mouseEvent->globalPos());
    }

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

void ToolBar::removeContextAction()
{
    removeAction(m_contextAction);
}

void ToolBar::showActionProp()
{
    ActionProp *actionProp = new ActionProp();
    actionProp->pushButtonIcon->setIcon(m_contextAction->icon());
    Menu *menu = qobject_cast<Menu *>(m_contextAction->menu());
    if (!menu)
        actionProp->groupBoxIcon->setDisabled(true);

    actionProp->lineEditName->setText(m_contextAction->text());
    if (actionProp->exec() == QDialog::Accepted) {
        m_contextAction->setText(actionProp->lineEditName->text());
        if (menu) {
            QIcon icon = actionProp->pushButtonIcon->icon();
            menu->setNativeIcon(actionProp->pushButtonIcon->data());
            m_contextAction->setIcon(icon);
        }
    }
    delete actionProp;
}

void ToolBar::setEdited(bool edited)
{
    m_edited = edited;
}

bool ToolBar::isEdited()
{
    return m_edited;
}
