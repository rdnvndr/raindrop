#include "menubar.h"
#include "menu.h"
#include <QAction>
#include <QDropEvent>
#include <QDebug>
#include "mimedataobject.h"

MenuBar::MenuBar(QWidget *parent) :
    QMenuBar(parent)
{
    setAcceptDrops(true);
}

void MenuBar::dropEvent(QDropEvent *event)
{
    const MimeDataObject *mimeData = qobject_cast<const MimeDataObject *>(event->mimeData());
    QAction *aAction = qobject_cast<QAction *>(mimeData->object());

    if (aAction) {
        QAction* eAction = this->actionAt(event->pos());
        if (aAction->menu())
            aAction = (new Menu("Новое меню"))->menuAction();

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
