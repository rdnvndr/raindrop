#include "treecombobox.h"
#include <QEvent>
#include <QMouseEvent>
#include <QHeaderView>
#include <QStylePainter>

namespace RTPTechGroup {
namespace Widgets {

TreeComboBox::TreeComboBox(QWidget *parent):QComboBox(parent),skipNextHide(false)
{
    m_rootIndex = QModelIndex();
    QComboBox::resize(250,30);
    treeView = new QTreeView();
    setView(treeView);
    view()->viewport()->installEventFilter(this);
    view()->setProperty("headerHidden",true);
    view()->setMinimumSize(0,150);
    view()->setAlternatingRowColors(true);

    m_indexColumn = 0;
    m_showingIcon = false;
}

TreeComboBox::~TreeComboBox()
{
    delete treeView;
}

bool TreeComboBox::eventFilter(QObject*object,QEvent*event)
{
    if(event->type()==QEvent::MouseButtonPress&&object==view()->viewport())
    {
        QMouseEvent*mouseEvent=static_cast<QMouseEvent*>(event);
        QModelIndex index=view()->indexAt(mouseEvent->pos());
        if(!view()->visualRect(index).contains(mouseEvent->pos()))
            skipNextHide=true;
    }
    return false;
}

void TreeComboBox::showPopup()
{
    QComboBox::showPopup();
}

void TreeComboBox::hidePopup()
{
    QComboBox::setRootModelIndex(view()->currentIndex().parent());
    setCurrentIndex(view()->currentIndex().row());

    if(skipNextHide)skipNextHide=false;
    else QComboBox::hidePopup();

    view()->setRootIndex(m_rootIndex);
}

void TreeComboBox::setModel(QAbstractItemModel *model)
{
    QComboBox::setModel(model);
    for (qint32 i = treeView->header()->count() - 1; i > 0; --i)
        treeView->hideColumn(i);
}

void TreeComboBox::setCurrentModelIndex(const QModelIndex &index)
{
    treeView->setCurrentIndex(index);
    hidePopup();
}

QModelIndex TreeComboBox::currentModelIndex()
{
    QModelIndex index = treeView->currentIndex();
    return index.sibling(index.row(),m_indexColumn);
}

void TreeComboBox::paintEvent(QPaintEvent *)
{
    QStylePainter painter(this);
    painter.setPen(palette().color(QPalette::Text));
    QStyleOptionComboBox opt;
    initStyleOption(&opt);
    if(currentIndex()==-1) {
        opt.currentText = m_displayText;
        opt.currentIcon = m_displayIcon;
    }
    if (!m_showingIcon)
        opt.currentIcon = QIcon();

    painter.drawComplexControl(QStyle::CC_ComboBox, opt);
    painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}


void TreeComboBox::setDisplayText(const QString &text)
{
    m_displayText = text;
    treeView->setCurrentIndex(treeView->currentIndex().sibling(-1,-1));
    this->repaint();
}

QString TreeComboBox::displayText() const
{
    return m_displayText;
}

void TreeComboBox::setDisplayIcon(const QIcon &icon)
{
    m_displayIcon = icon;
}

QIcon TreeComboBox::displayIcon() const
{
    return m_displayIcon;
}

void TreeComboBox::setIndexColumn(qint32 column)
{
    m_indexColumn = column;
}

void TreeComboBox::setShowingIcon(bool showing)
{
    m_showingIcon = showing;
}

bool TreeComboBox::showingIcon()
{
    return m_showingIcon;
}

void TreeComboBox::setRootModelIndex(const QModelIndex &index)
{
    m_rootIndex = index;
}

QModelIndex TreeComboBox::rootModelIndex()
{
    return m_rootIndex;
}

}}
