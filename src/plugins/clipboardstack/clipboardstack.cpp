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

    // Создание пунктов строки меню и кнопок панели иструментов
    IMainWindow *iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

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
    m_clipboardItemList.clear();
}

void ClipboardStack::addClipboardItem(IClipboardItem *item)
{
    QObject *itemObject = dynamic_cast<QObject *>(item);
    m_clipboardItemList.append(itemObject);
    connect(itemObject, SIGNAL(canCutChanged(bool)),
            this, SLOT(canCutChange(bool)));
    connect(itemObject, SIGNAL(canCopyChanged(bool)),
            this, SLOT(canCopyChange(bool)));
    connect(itemObject, SIGNAL(canPasteChanged(bool)),
            this, SLOT(canPasteChange(bool)));
    connect(itemObject, SIGNAL(canSelectAllChanged(bool)),
            this, SLOT(canSelectAllChange(bool)));
    connect(itemObject, SIGNAL(destroyed(QObject*)),
            this, SLOT(removeItem(QObject*)));
}

void ClipboardStack::removeClipboardItem(IClipboardItem *item)
{
    removeItem(dynamic_cast<QObject *>(item));
}

void ClipboardStack::focusChanged(QWidget *old, QWidget *now)
{
    Q_UNUSED(old)

    if (now == 0)
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

void ClipboardStack::canCutChange(bool canCut)
{
    if (canCut)
        m_actionCut->setEnabled(true);
    else
        m_actionCut->setEnabled(false);
}

void ClipboardStack::canCopyChange(bool canCopy)
{
    if (canCopy)
        m_actionCopy->setEnabled(true);
    else
        m_actionCopy->setEnabled(false);
}

void ClipboardStack::canPasteChange(bool canPaste)
{
    if (canPaste)
        m_actionPaste->setEnabled(true);
    else
        m_actionPaste->setEnabled(false);
}

void ClipboardStack::canSelectAllChange(bool canSelectAll)
{
    if (canSelectAll)
        m_actionSelectAll->setEnabled(true);
    else
        m_actionSelectAll->setEnabled(false);
}

void ClipboardStack::clipboardDataChange()
{
    setActiveItemForWidget(qApp->focusWidget());
}

void ClipboardStack::removeItem(QObject *obj)
{
    m_clipboardItemList.removeAll(obj);
}

void ClipboardStack::setActiveItemForWidget(QWidget *widget)
{
    Q_UNUSED(widget)
    QListIterator<QObject *> i(m_clipboardItemList);
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
    m_currentClipboardItem = NULL;
    m_actionCut->setEnabled(false);
    m_actionCopy->setEnabled(false);
    m_actionPaste->setEnabled(false);
    m_actionSelectAll->setEnabled(false);
}

}}
