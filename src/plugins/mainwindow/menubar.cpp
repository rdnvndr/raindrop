#include "menubar.h"
#include "menu.h"
#include <QAction>
#include <QDropEvent>
#include <QApplication>
#include <QDebug>
#include <QDrag>
#include "mimedataobject.h"
#include "actionprop.h"

namespace RTPTechGroup {
namespace MainWindow {

MenuBar::MenuBar(QWidget *parent) :
    QMenuBar(parent)
{
    setAcceptDrops(true);
    m_dragPos = QPoint(-1,-1);
    m_edited = false;
    m_contextAction = nullptr;
}

MenuBar::~MenuBar()
{

}

void MenuBar::mouseMoveEvent(QMouseEvent *event)
{
    QMenuBar::mouseMoveEvent(event);
    QAction *action  = this->actionAt(m_dragPos);

    if (event->buttons() & Qt::LeftButton && action && isEdited()) {
        qint32 distance = (event->pos() - m_dragPos).manhattanLength();
        if (distance > QApplication::startDragDistance() ) {
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
    }

    QMenuBar::mousePressEvent(event);
}

void MenuBar::dropEvent(QDropEvent *event)
{
    const MimeDataObject *mimeData = qobject_cast<const MimeDataObject *>(event->mimeData());
    QAction *aAction = qobject_cast<QAction *>(mimeData->object());

    if (aAction && isEdited()) {
        if (activeAction())
            if (activeAction()->menu())
                activeAction()->menu()->close();

        if (aAction->menu())
            if (aAction->objectName() == "actionNewMenu") {
                Menu *menu =  new Menu(aAction->text());
                menu->setEdited(true);
                aAction = menu->menuAction();
            }

        QAction *eAction = this->actionAt(event->pos());
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
    if (event->mimeData()->hasFormat("application/x-qobject")
            && isEdited())
        event->acceptProposedAction();
     m_dragPos = QPoint(-1,-1);
}

void MenuBar::dragMoveEvent(QDragMoveEvent *event)
{
    const MimeDataObject *mimeData
            = qobject_cast<const MimeDataObject *>(event->mimeData());

    QAction *eAction = this->actionAt(event->pos());
    if (mimeData->hasFormat("application/x-qobject") && isEdited())
        if (mimeData->object() != eAction && eAction)
            if (eAction->menu() && activeAction()!= eAction)
                setActiveAction(eAction);
    event->accept();
}

void MenuBar::contextMenuEvent(QContextMenuEvent *event)
{
    if (this->actionAt(event->pos()) != nullptr && isEdited()) {
        // Создание контекстного меню
        QMenu *contextMenu = new QMenu();
        QAction *action = new QAction(tr("Удалить"),this);
        connect(action, &QAction::triggered, this, &MenuBar::removeContextAction);
        contextMenu->addAction(action);
        contextMenu->addSeparator();
        action = new QAction(tr("Свойства..."),this);
        connect(action, &QAction::triggered, this, &MenuBar::showActionProp);
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

void MenuBar::setEdited(bool edited)
{
    m_edited = edited;
}

bool MenuBar::isEdited()
{
    return m_edited;
}

}
}
