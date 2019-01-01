#include "clipboardstack.h"

#include <imainwindow.h>
#include <QClipboard>

using namespace RTPTechGroup::Plugin;
namespace RTPTechGroup {
namespace StackEditing {

ClipboardStack::ClipboardStack(QObject *parent):
    QObject(parent), IPlugin("IMainWindow")
{
    PluginManager *pluginManager = PluginManager::instance();

    // Создание пунктов строки меню и кнопок панели инструментов
    IMainWindow *iMainWindow
            = pluginManager->interfaceObject<IMainWindow*>("IMainWindow");
    if (iMainWindow == nullptr)
        return;

    connect(qApp, &QApplication::focusChanged,
            this, &ClipboardStack::focusChanged);

    connect(qApp->clipboard(), &QClipboard::dataChanged,
            this, &ClipboardStack::clipboardDataChange);

    m_actionCut = new QAction(QIcon(":cut"), tr("Вырезать"), this);
    m_actionCut->setObjectName("actionCut");
    connect(m_actionCut, &QAction::triggered, this, &ClipboardStack::cut);
    iMainWindow->addAction(tr("Редактирование"), m_actionCut);

    m_actionCopy = new QAction(QIcon(":copy"), tr("Копировать"), this);
    m_actionCopy->setObjectName("actionCopy");
    connect(m_actionCopy, &QAction::triggered, this, &ClipboardStack::copy);
    iMainWindow->addAction(tr("Редактирование"), m_actionCopy);

    m_actionPaste = new QAction(QIcon(":paste"), tr("Вставить"), this);
    m_actionPaste->setObjectName("actionPaste");
    connect(m_actionPaste, &QAction::triggered, this, &ClipboardStack::paste);
    iMainWindow->addAction(tr("Редактирование"), m_actionPaste);

    m_actionSelectAll = new QAction(QIcon(":selectall"), tr("Выделить всё"), this);
    m_actionSelectAll->setObjectName("actionSelectAll");
    connect(m_actionSelectAll, &QAction::triggered, this, &ClipboardStack::selectAll);
    iMainWindow->addAction(tr("Редактирование"), m_actionSelectAll);

}

ClipboardStack::~ClipboardStack()
{

}

void ClipboardStack::focusChanged(QWidget *old, QWidget *now)
{
    Q_UNUSED(old)

    if (now == nullptr)
        return;

    setActiveItemForWidget(now);
}

void ClipboardStack::cut()
{
    if (m_currentClipboardItem)
        m_currentClipboardItem->cut();
}

void ClipboardStack::copy()
{
    if (m_currentClipboardItem)
        m_currentClipboardItem->copy();
}

void ClipboardStack::paste()
{
    if (m_currentClipboardItem)
        m_currentClipboardItem->paste();
}

void ClipboardStack::selectAll()
{
    if (m_currentClipboardItem)
        m_currentClipboardItem->selectAll();
}

void ClipboardStack::enableCut(bool enabled)
{
    if (enabled)
        m_actionCut->setEnabled(true);
    else
        m_actionCut->setEnabled(false);
}

void ClipboardStack::enableCopy(bool enabled)
{
    if (enabled)
        m_actionCopy->setEnabled(true);
    else
        m_actionCopy->setEnabled(false);
}

void ClipboardStack::enablePaste(bool enabled)
{
    if (enabled)
        m_actionPaste->setEnabled(true);
    else
        m_actionPaste->setEnabled(false);
}

void ClipboardStack:: enableSelectAll(bool enabled)
{
    if (enabled)
        m_actionSelectAll->setEnabled(true);
    else
        m_actionSelectAll->setEnabled(false);
}

void ClipboardStack::clipboardDataChange()
{
    setActiveItemForWidget(qApp->focusWidget());
}

void ClipboardStack::setActiveItemForWidget(QWidget *widget)
{
    Q_UNUSED(widget)
    QListIterator<IClipboardElement *> i(m_elementList);
    i.toBack();
    while (i.hasPrevious()) {
        IClipboardItem *item = dynamic_cast<IClipboardItem *>(i.previous());
        bool isCut       = item->canCut();
        bool isCopy      = item->canCopy();
        bool isPaste     = item->canPaste();
        bool isSelectAll = item->canSelectAll();
        if (isCut || isCopy || isPaste || isSelectAll) {
            m_currentClipboardItem = item;
            m_actionCut->setEnabled(isCut);
            m_actionCopy->setEnabled(isCopy);
            m_actionPaste->setEnabled(isPaste);
            m_actionSelectAll->setEnabled(isSelectAll);
            return;
        }
    }
    m_currentClipboardItem = nullptr;
    m_actionCut->setEnabled(false);
    m_actionCopy->setEnabled(false);
    m_actionPaste->setEnabled(false);
    m_actionSelectAll->setEnabled(false);
}

}}
