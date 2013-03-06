#include <QByteArray>
#include <QDebug>
#include <QAction>
#include <QMenuBar>
#include <QDropEvent>
#include <QDrag>
#include <QApplication>
#include <QPainter>
#include "menu.h"
#include "actionprop.h"
#include "mimedataobject.h"

Menu::Menu(QWidget *parent) :
    QMenu(parent)
{
    setAcceptDrops(true);    
    m_dragPos = QPoint(-1,-1);
    setContextMenuPolicy(Qt::NoContextMenu);
    m_edited = false;
}

Menu::Menu(const QString &title, QWidget *parent):
    QMenu(title, parent)
{
    setAcceptDrops(true);
    m_dragPos = QPoint(-1,-1);
    setContextMenuPolicy(Qt::NoContextMenu);
    m_edited = false;
}

Menu::~Menu() {

}

void Menu::dropEvent(QDropEvent *event)
{
    if (!isEdited())
        return;

    const MimeDataObject *mimeData = qobject_cast<const MimeDataObject *>(event->mimeData());
    QAction *aAction = qobject_cast<QAction *>(mimeData->object());

    if (aAction) {
        if (aAction->menu())
            if (aAction->objectName() == "actionNewMenu") {
                Menu *menu =  new Menu(aAction->text());
                menu->setEdited(true);
                aAction = menu->menuAction();
            }

        QAction* eAction = this->actionAt(event->pos());
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

void Menu::dragEnterEvent(QDragEnterEvent *event)
{
    if ((qobject_cast<const MimeDataObject *>(event->mimeData()))->hasObject()
            && isEdited())
        event->acceptProposedAction();
    m_dragPos = QPoint(-1,-1);
}

void Menu::dragMoveEvent(QDragMoveEvent *event)
{
    const MimeDataObject *mimeData
            = qobject_cast<const MimeDataObject *>(event->mimeData());

    QAction* eAction = this->actionAt(event->pos());
    if (mimeData->hasFormat("application/x-qobject") && isEdited())
        if (mimeData->object() != eAction && eAction)
            if (eAction->menu() && activeAction()!= eAction)
                setActiveAction(eAction);
    event->accept();
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

void Menu::setEdited(bool edited)
{
    m_edited = edited;
}

bool Menu::isEdited()
{
    return m_edited;
}

void Menu::removeContextAction()
{
    removeAction(m_contextAction);
}

void Menu::showActionProp()
{
    ActionProp *actionProp = new ActionProp();
    actionProp->lineEditName->setText(m_contextAction->text());
    if (actionProp->exec() == QDialog::Accepted) {
        m_contextAction->setText(actionProp->lineEditName->text());
    }
}

void Menu::mouseMoveEvent(QMouseEvent *event)
{
    QMenu::mouseMoveEvent(event);
    QAction *action = this->actionAt(m_dragPos);
    if (event->buttons() & Qt::LeftButton && action && isEdited()) {
        int distance = (event->pos() - m_dragPos).manhattanLength();
        if (distance > QApplication::startDragDistance()) {
            qDebug() << "Menu:" <<m_dragPos;
            qDebug() << "drag menu";
            if (action->menu())
                action->menu()->close();

            QDrag *drag = new QDrag(this);
            MimeDataObject *mimeData = new MimeDataObject();

/*
            QRect rect = actionGeometry(action);
            drag->setHotSpot(m_dragPos - rect.topLeft());
            QPixmap pixmap(rect.width(),rect.height());
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            QRegion region(rect);
            painter.begin(&pixmap);
            render(&painter,QPoint(),region,DrawChildren);
            painter.end();
            drag->setPixmap(pixmap);
*/
            mimeData->setObject(action);
            drag->setMimeData(mimeData);

            if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
                if (drag->target() != this || action->isSeparator())
                    this->removeAction(action);
        }
    }
}

void Menu::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_dragPos = event->pos();

    // Вызов контекстного меню, contextMenuEvent() не вызывается для QAction типа меню)
    if (event->button() == Qt::RightButton && isEdited()) {
        m_contextAction = this->actionAt(event->pos());
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
            contextMenu->exec(event->globalPos());
            delete contextMenu;
        }
    }

    QMenu::mousePressEvent(event);
}
