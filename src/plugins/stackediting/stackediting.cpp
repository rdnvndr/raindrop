#include "stackediting.h"

#include <imainwindow.h>
#include <QClipboard>

using namespace RTPTechGroup::Plugin;
namespace RTPTechGroup {
namespace StackEditing {


StackEditing::StackEditing(QObject *parent):
    QObject(parent), IPlugin("IMainWindow")
{
    m_undoGroup = new QUndoGroup(this);

    PluginManager *pluginManager = PluginManager::instance();

    // Создание пунктов строки меню и кнопок панели иструментов
    IMainWindow *iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    actionUndo = m_undoGroup->createUndoAction(this);
    actionUndo->setIcon(QIcon(":undo"));
    actionUndo->setObjectName("actionUndo");    
    iMainWindow->addAction(tr("Редактирование"), actionUndo);

    actionRedo = m_undoGroup->createRedoAction(this);
    actionRedo->setIcon(QIcon(":redo"));
    actionRedo->setObjectName("actionRedo");
    iMainWindow->addAction(tr("Редактирование"), actionRedo);

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(focusChanged(QWidget*,QWidget*)));

    connect(qApp->clipboard(), SIGNAL(dataChanged()),
            this, SLOT(clipboardDataChange()));

    actionCut = new QAction(QIcon(":cut"), tr("Вырезать"), this);
    actionCut->setObjectName("actionCut");
    connect(actionCut, SIGNAL(triggered()), this, SLOT(cut()));
    iMainWindow->addAction(tr("Редактирование"), actionCut);

    actionCopy = new QAction(QIcon(":copy"), tr("Копировать"), this);
    actionCopy->setObjectName("actionCopy");
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
    iMainWindow->addAction(tr("Редактирование"), actionCopy);

    actionPaste = new QAction(QIcon(":paste"), tr("Вставить"), this);
    actionPaste->setObjectName("actionPaste");
    connect(actionPaste, SIGNAL(triggered()), this, SLOT(paste()));
    iMainWindow->addAction(tr("Редактирование"), actionPaste);

    actionSelectAll = new QAction(QIcon(":selectall"), tr("Выделить всё"), this);
    actionSelectAll->setObjectName("actionSelectAll");
    connect(actionSelectAll, SIGNAL(triggered()), this, SLOT(selectAll()));
    iMainWindow->addAction(tr("Редактирование"), actionSelectAll);
}

StackEditing::~StackEditing()
{
    m_clipboardItemList.clear();
    m_undoStackList.clear();
    delete m_undoGroup;
}

void StackEditing::addClipboardItem(IClipboardItem *item)
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

void StackEditing::removeClipboardItem(IClipboardItem *item)
{
    removeItem(dynamic_cast<QObject *>(item));
}

void StackEditing::addStack(QUndoStack *stack)
{
    m_undoGroup->addStack(stack);
    connect(stack, SIGNAL(destroyed(QObject*)), this, SLOT(removeStack(QObject*)));
}

void StackEditing::removeStack(QUndoStack *stack)
{
    m_undoStackList.remove(stack);
    m_undoGroup->removeStack(stack);
}

void StackEditing::addWidgetForStack(QUndoStack *stack, QWidget *widget)
{
    m_undoStackList.insert(stack, widget);
}

void StackEditing::removeWidgetForStack(QWidget *widget)
{
    foreach (QUndoStack *undoStack, m_undoStackList.keys(widget))
        m_undoStackList.remove(undoStack, widget);
}

void StackEditing::focusChanged(QWidget *old, QWidget *now)
{
    Q_UNUSED(old)

    if (now == 0)
        return;

    setActiveItemForWidget(now);
    setActiveStackForWidget(now);
}

void StackEditing::removeStack(QObject *obj)
{
    QUndoStack *stack = static_cast<QUndoStack *>(obj);
    removeStack(stack);
}

void StackEditing::cut()
{
    if (m_currentClipboardItem)
        m_currentClipboardItem->cut();
}

void StackEditing::copy()
{
    if (m_currentClipboardItem)
        m_currentClipboardItem->copy();
}

void StackEditing::paste()
{
    if (m_currentClipboardItem)
        m_currentClipboardItem->paste();
}

void StackEditing::selectAll()
{
    if (m_currentClipboardItem)
        m_currentClipboardItem->selectAll();
}

void StackEditing::canCutChange(bool canCut)
{
    if (canCut)
        actionCut->setEnabled(true);
    else
        actionCut->setEnabled(false);
}

void StackEditing::canCopyChange(bool canCopy)
{
    if (canCopy)
        actionCopy->setEnabled(true);
    else
        actionCopy->setEnabled(false);
}

void StackEditing::canPasteChange(bool canPaste)
{
    if (canPaste)
        actionPaste->setEnabled(true);
    else
        actionPaste->setEnabled(false);
}

void StackEditing::canSelectAllChange(bool canSelectAll)
{
    if (canSelectAll)
        actionSelectAll->setEnabled(true);
    else
        actionSelectAll->setEnabled(false);
}

void StackEditing::clipboardDataChange()
{
    setActiveItemForWidget(qApp->focusWidget());
}

void StackEditing::removeItem(QObject *obj)
{
    m_clipboardItemList.removeAll(obj);
}

void StackEditing::setActiveStackForWidget(QWidget *widget)
{
    // Стек отмены/повтора команд
    QMutableMapIterator<QUndoStack *, QWidget *> i(m_undoStackList);
    i.toBack();
    while (i.hasPrevious()) {
        i.previous();
        if (i.value()->isAncestorOf(widget)) {
            m_undoGroup->setActiveStack(i.key());
            return;
        }
    }
    m_undoGroup->setActiveStack(NULL);
}

void StackEditing::setActiveItemForWidget(QWidget *widget)
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
            actionCut->setEnabled(isCut);
            actionCopy->setEnabled(isCopy);
            actionPaste->setEnabled(isPaste);
            actionSelectAll->setEnabled(isSelectAll);
            return;
        }
    }
    m_currentClipboardItem = NULL;
    actionCut->setEnabled(false);
    actionCopy->setEnabled(false);
    actionPaste->setEnabled(false);
    actionSelectAll->setEnabled(false);
}

}}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(undostack, UndoStack)
#endif



