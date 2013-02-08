#include "menu.h"
#include <QByteArray>
#include <QDebug>
#include <QAction>
#include <QDropEvent>
#include "mimedataobject.h"

Menu::Menu(QWidget *parent) :
    QMenu(parent)
{
    setAcceptDrops(true);
}

Menu::Menu(const QString &title, QWidget *parent):
    QMenu(title, parent)
{
    setAcceptDrops(true);
}

void Menu::dropEvent(QDropEvent *event)
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

void Menu::dragEnterEvent(QDragEnterEvent *event)
{
    if ((qobject_cast<const MimeDataObject *>(event->mimeData()))->hasObject())
        event->acceptProposedAction();
}

void Menu::dragMoveEvent(QDragMoveEvent *event)
{
    QAction* eAction = this->actionAt(event->pos());
    if (eAction)
        if (eAction->menu() && activeAction()!=eAction)
            setActiveAction(eAction);
}

QSize Menu::sizeHint() const
{
    QSize size =  QMenu::sizeHint();

    if ((size.width() < 3) && (size.height() < 3)) {
        size.setHeight(25);
        size.setWidth(75);
        return size;
    }

    return QMenu::sizeHint();
}
