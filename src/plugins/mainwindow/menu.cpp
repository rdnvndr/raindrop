#include "menu.h"
#include <QByteArray>
#include <QDebug>
#include <QAction>
#include <QDropEvent>
#include <QApplication>
#include <QPainter>
#include "mimedataobject.h"

Menu::Menu(QWidget *parent) :
    QMenu(parent)
{
    setAcceptDrops(true);

    // Создание контекстного меню
    m_contextMenu = new QMenu();

    QAction *action = new QAction(tr("Добавить"),this);
    m_contextMenu->addAction(action);
}

Menu::Menu(const QString &title, QWidget *parent):
    QMenu(title, parent)
{
    setAcceptDrops(true);

    // Создание контекстного меню
    m_contextMenu = new QMenu();

    QAction *action = new QAction(tr("Добавить"),this);
    m_contextMenu->addAction(action);
}

Menu::~Menu() {
    delete m_contextMenu;
}

void Menu::dropEvent(QDropEvent *event)
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

void Menu::contextMenuEvent(QContextMenuEvent *event)
{
    m_contextMenu->exec(event->globalPos());
}

void Menu::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - m_dragPos).manhattanLength();
        if (distance > QApplication::startDragDistance()) {

            QDrag *drag = new QDrag(this);
            MimeDataObject *mimeData = new MimeDataObject();
            QAction *action  = this->actionAt(m_dragPos);
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
                if (drag->target() != this)
                    this->removeAction(action);
        }
    }
    QMenu::mouseMoveEvent(event);
}

void Menu::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_dragPos = event->pos();

    QMenu::mousePressEvent(event);
}
