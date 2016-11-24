#include "tilingframe.h"

namespace RTPTechGroup {
namespace Widgets {

TilingFrame::TilingFrame(TilingWidget *widget)
{
    m_layout = new QStackedLayout(this);
    m_layout->setSizeConstraint(QLayout::SetNoConstraint);
    m_splitter = NULL;
    m_parentFrame = NULL;

    if (widget) {
        m_tilingWidget = widget;
        m_tilingWidget->setTilingFrame(this);
        m_layout->addWidget(dynamic_cast<QWidget *>(m_tilingWidget));
    }
}

TilingFrame::~TilingFrame()
{
    delete m_layout;
    m_layout = NULL;
    delete m_tilingWidget;
    m_tilingWidget = NULL;
    delete m_splitter;
    m_splitter = NULL;
    m_parentFrame = NULL;
}

bool TilingFrame::isWidget() const
{
    return m_tilingWidget != NULL;
}

TilingWidget *TilingFrame::widget() const
{
    return m_tilingWidget;
}

bool TilingFrame::isSplitter() const
{
    return m_splitter != NULL;
}

QSplitter *TilingFrame::splitter() const
{
    return m_splitter;
}

void TilingFrame::split(Qt::Orientation orientation)
{
    if (m_splitter)
        return;

    m_splitter = new QSplitter(this);
    m_splitter->setOrientation(orientation);
    m_layout->addWidget(m_splitter);
    m_layout->removeWidget(dynamic_cast<QWidget *>(m_tilingWidget));
    TilingWidget *widget = m_tilingWidget;
    m_tilingWidget = NULL;

    TilingFrame *parentFrame = new TilingFrame(widget);
    m_splitter->addWidget(parentFrame);
    parentFrame->m_parentFrame = this;
    parentFrame = new TilingFrame(widget->clone());
    parentFrame->m_parentFrame = this;
    m_splitter->addWidget(parentFrame);

    m_layout->setCurrentWidget(m_splitter);

    emit splitStateChanged();
}

void TilingFrame::splitHorizontal()
{
    split(Qt::Horizontal);
}

void TilingFrame::splitVertical()
{
    split(Qt::Vertical);
}

void TilingFrame::unsplit(TilingFrame *unsplitFrame)
{
    if (!m_splitter) {
        TilingFrame *parentSplitter = findParentSplitter();
        if (parentSplitter)
            parentSplitter->unsplit(this);
        return;
    }

    TilingFrame *childTilingFrame = qobject_cast<TilingFrame *>(m_splitter->widget(1));
    if (unsplitFrame == childTilingFrame)
        childTilingFrame = qobject_cast<TilingFrame *>(m_splitter->widget(0));

    QSplitter *oldSplitter = m_splitter;
    m_splitter = NULL;

    if (childTilingFrame->isSplitter()) {
        m_splitter = childTilingFrame->takeSplitter();
        m_layout->addWidget(m_splitter);
        m_layout->setCurrentWidget(m_splitter);

        childTilingFrame = qobject_cast<TilingFrame *>(m_splitter->widget(0));
        childTilingFrame->m_parentFrame = this;
        childTilingFrame = qobject_cast<TilingFrame *>(m_splitter->widget(1));
        childTilingFrame->m_parentFrame = this;
    } else {
        TilingWidget *childWidget = childTilingFrame->takeWidget();
        if (childWidget) {
            childWidget->setTilingFrame(this);
            QWidget *widget = dynamic_cast<QWidget *>(childWidget);
            m_tilingWidget = childWidget;
            m_layout->addWidget(widget);
            m_layout->setCurrentWidget(widget);
        }
    }
    delete oldSplitter;

    emit splitStateChanged();
}

void TilingFrame::unsplitAll()
{
    if (m_splitter)
        return;

    TilingFrame *parentPrevFrame = NULL;
    TilingFrame *parentFrame = this;
    while (parentFrame->parentFrame()) {
        parentPrevFrame = parentFrame;
        parentFrame = parentFrame->parentFrame();
    }

    if (!parentPrevFrame || !parentFrame->isSplitter())
        return;

    TilingWidget *childWidget = this->takeWidget();
    if (childWidget) {
        childWidget->setTilingFrame(parentFrame);
        QWidget *widget = dynamic_cast<QWidget *>(childWidget);
        parentFrame->m_tilingWidget = childWidget;

        parentFrame->m_layout->addWidget(widget);
        parentFrame->m_layout->setCurrentWidget(widget);
    }
    delete parentPrevFrame;
    delete parentFrame->m_splitter;
    parentFrame->m_splitter = NULL;

    emit splitStateChanged();
}

TilingWidget *TilingFrame::takeWidget()
{
    TilingWidget *oldWidget = m_tilingWidget;
    if (m_tilingWidget) {
        m_layout->removeWidget(dynamic_cast<QWidget *>(m_tilingWidget));
    }
    m_tilingWidget = NULL;
    return oldWidget;
}

QSplitter *TilingFrame::takeSplitter()
{
    QSplitter *oldSplitter = m_splitter;
    if (m_splitter)
        m_layout->removeWidget(m_splitter);
    m_splitter = NULL;
    return oldSplitter;
}

TilingFrame *TilingFrame::findParentSplitter() const
{
    TilingFrame *parentFrame = m_parentFrame;
    while (parentFrame) {
        if (parentFrame->isSplitter()) {
            return parentFrame;
        }
        parentFrame = parentFrame->parentFrame();
    }
    return NULL;
}

TilingFrame *TilingFrame::parentFrame() const
{
    return m_parentFrame;
}

}}
