#include "menubar.h"
#include "menu.h"
#include <QAction>
#include <QDropEvent>
#include <QApplication>
#include <QDebug>
#include <QDrag>
#include "mimedataobject.h"
#include "actionprop.h"

MenuBar::MenuBar(QWidget *parent) :
    QMenuBar(parent)
{
    setAcceptDrops(true);
    m_dragPos = QPoint(-1,-1);
}

MenuBar::~MenuBar()
{

}

void MenuBar::mouseMoveEvent(QMouseEvent *event)
{
    QMenuBar::mouseMoveEvent(event);
    QAction *action  = this->actionAt(m_dragPos);

    if (event->buttons() & Qt::LeftButton && action) {
        int distance = (event->pos() - m_dragPos).manhattanLength();
        if (distance > QApplication::startDragDistance() ) {
            qDebug() << "MenuBar:" <<m_dragPos;
            qDebug() << "drag menubar";
            if (action->menu()) {
                action->menu()->close();
            }

            QDrag *drag = new QDrag(this);
            MimeDataObject *mimeData = new MimeDataObject();

            mimeData->setObject(action);
            drag->setMimeData(mimeData);

            if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
                if (drag->target() != this)
                    this->removeAction(action);
        }
    }
}

void MenuBar::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton) {
        m_dragPos = event->pos();
        qDebug() << "Press menubar";
    }

    QMenuBar::mousePressEvent(event);
}

void MenuBar::dropEvent(QDropEvent *event)
{
    const MimeDataObject *mimeData = qobject_cast<const MimeDataObject *>(event->mimeData());
    QAction *aAction = qobject_cast<QAction *>(mimeData->object());

    if (aAction) {
        if (activeAction())
            if (activeAction()->menu())
                activeAction()->menu()->close();

        if (aAction->menu())
            if (aAction->objectName() == "actionNewMenu") {
                aAction = (new Menu(aAction->text()))->menuAction();
            }

        QAction* eAction = this->actionAt(event->pos());
        QRect rect = actionGeometry(eAction);
        eAction = this->actionAt(QPoint(event->pos().x()+rect.width()/2,
                                        event->pos().y()));
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

void MenuBar::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qobject"))
        event->acceptProposedAction();
     m_dragPos = QPoint(-1,-1);
}

void MenuBar::dragMoveEvent(QDragMoveEvent *event)
{
    const MimeDataObject *mimeData
            = qobject_cast<const MimeDataObject *>(event->mimeData());

    QAction* eAction = this->actionAt(event->pos());
    if (mimeData->hasFormat("application/x-qobject"))
        if (mimeData->object() != eAction && eAction)
            if (eAction->menu() && activeAction()!= eAction)
                setActiveAction(eAction);
    event->accept();
}

void MenuBar::contextMenuEvent(QContextMenuEvent *event)
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
        QMenuBar::contextMenuEvent(event);
}

void MenuBar::removeContextAction()
{
    removeAction(m_contextAction);
}

void MenuBar::showActionProp()
{
    ActionProp *actionProp = new ActionProp();
    actionProp->lineEditName->setText(m_contextAction->text());
    if (actionProp->exec() == QDialog::Accepted) {
        m_contextAction->setText(actionProp->lineEditName->text());
    }
}
