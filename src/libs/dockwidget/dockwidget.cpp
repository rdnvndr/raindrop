#include "dockwidget.h"
#include <QEvent>
#include <QApplication>
#include <QMouseEvent>
#include <QStyle>

namespace RTPTechGroup {
namespace Widgets {

DockWidget::DockWidget(QWidget *parent)
    : QDockWidget(parent)
{
    m_titleBar = titleBarWidget();
    m_emptyTitleBar = new QWidget(this);
    setMouseTracking(true);
    setAutoHide(true);

    connect(&m_timer, &QTimer::timeout, this, &DockWidget::handleMouseTimeout);
    connect(this, &QDockWidget::topLevelChanged, this, &DockWidget::handleToplevelChanged);

    setMinimumWidth(0);
}

DockWidget::~DockWidget()
{
    delete m_emptyTitleBar;
}

bool DockWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove && !isFloating() && isAutoHide()) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        QWidget *widget = qobject_cast<QWidget *>(obj);
        if (widget) {
            QPoint globalPos = widget->mapToGlobal(me->pos());
            QPoint localPos  = mapFromGlobal(globalPos);
            qint32 y = localPos.y();
            qint32 x = localPos.x();
            qint32 h = style()->pixelMetric(QStyle::PM_TitleBarHeight);

            if (0 <= x && x < width() && 0 <= y && y <= h) {
                if (!isTitleBarVisible()) {
                    m_timer.start();
                    m_startPos = globalPos;
                }
            } else if (isTitleBarVisible()) {
                setTitleBarVisible(false);
            }
        }
    }

        return false;
}

void DockWidget::enterEvent(QEvent *event)
{
    QApplication::instance()->installEventFilter(this);
    QDockWidget::enterEvent(event);
}

void DockWidget::leaveEvent(QEvent *event)
{
    QApplication::instance()->removeEventFilter(this);
    QDockWidget::leaveEvent(event);
}

void DockWidget::handleMouseTimeout()
{
    QPoint dist = m_startPos - QCursor::pos();
    if (!isFloating() && dist.manhattanLength() < 4 && isAutoHide())
        setTitleBarVisible(true);
}

void DockWidget::handleToplevelChanged(bool floating)
{
    setTitleBarVisible(floating);
}

void DockWidget::setMinimumWidth(qint32 minw)
{
    QDockWidget::setMinimumWidth((minw<25)?25:minw);
}

void DockWidget::setTitleBarVisible(bool visible)
{
    if (visible)
        this->setTitleBarWidget(NULL);
    else
        this->setTitleBarWidget(m_emptyTitleBar);
}

bool DockWidget::isTitleBarVisible()
{
    return (m_emptyTitleBar == titleBarWidget()) ? false : true;
}

void DockWidget::setTitleBarWidget(QWidget *widget)
{
    m_titleBar = widget;
    QDockWidget::setTitleBarWidget(widget);
}

void DockWidget::setAutoHide(bool autoHide)
{
    if (autoHide) {
        setTitleBarVisible(false);
        m_timer.setSingleShot(true);
        m_timer.setInterval(500);
    }
    m_autoHide = autoHide;
}

bool DockWidget::isAutoHide()
{
    return m_autoHide;
}

}}
