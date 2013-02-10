#include "menubar.h"
#include "menu.h"
#include <QAction>
#include <QDropEvent>
#include <QApplication>
#include <QDebug>
#include "mimedataobject.h"

MenuBar::MenuBar(QWidget *parent) :
    QMenuBar(parent)
{
    setAcceptDrops(true);

    // Создание контекстного меню
    m_contextMenu = new QMenu();

    QAction *action = new QAction(tr("Добавить"),this);
    m_contextMenu->addAction(action);
}

MenuBar::~MenuBar()
{
    delete m_contextMenu;
}

void MenuBar::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - m_dragPos).manhattanLength();
        if (distance > QApplication::startDragDistance()) {
            QDrag *drag = new QDrag(this);
            MimeDataObject *mimeData = new MimeDataObject();
            QAction *action  = this->actionAt(m_dragPos);

            mimeData->setObject(action);
            drag->setMimeData(mimeData);

            if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
                this->removeAction(action);
        }
    }
    QMenuBar::mouseMoveEvent(event);
}

void MenuBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_dragPos = event->pos();

    QMenuBar::mousePressEvent(event);
}

void MenuBar::dropEvent(QDropEvent *event)
{
    const MimeDataObject *mimeData = qobject_cast<const MimeDataObject *>(event->mimeData());
    QAction *aAction = qobject_cast<QAction *>(mimeData->object());

    if (aAction) {
        QAction* eAction = this->actionAt(event->pos());
        if (aAction->menu())
            aAction = (new Menu("Новое меню"))->menuAction();

        QRect rect = actionGeometry(eAction);

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

void MenuBar::dragEnterEvent(QDragEnterEvent *event)
{
    if ((qobject_cast<const MimeDataObject *>(event->mimeData()))->hasObject())
        event->acceptProposedAction();
}

void MenuBar::dragMoveEvent(QDragMoveEvent *event)
{
    QAction* eAction = this->actionAt(event->pos());
    if (eAction)
        if (eAction->menu() && activeAction()!= eAction)
            setActiveAction(eAction);
}

void MenuBar::contextMenuEvent(QContextMenuEvent *event)
{
   m_contextMenu->exec(event->globalPos());
}
