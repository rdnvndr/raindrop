#include "tilingwidget.h"

namespace RTPTechGroup {
namespace Widgets {

TilingWidget::TilingWidget(QWidget *parent)
    :QWidget (parent)
{
    m_tilingFrame = NULL;
}

TilingWidget::~TilingWidget()
{

}

void TilingWidget::split(Qt::Orientation orientation)
{
    m_tilingFrame->split(orientation);
}

void TilingWidget::splitHorizontal()
{
    m_tilingFrame->splitHorizontal();
}

void TilingWidget::splitVertical()
{
    m_tilingFrame->splitVertical();
}

void TilingWidget::unsplit()
{
    m_tilingFrame->unsplit();
}

void TilingWidget::unsplitAll()
{
    m_tilingFrame->unsplitAll();
}

}}
