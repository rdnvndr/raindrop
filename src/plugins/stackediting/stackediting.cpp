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

    m_actionUndo = m_undoGroup->createUndoAction(this);
    m_actionUndo->setIcon(QIcon(":undo"));
    m_actionUndo->setObjectName("actionUndo");
    iMainWindow->addAction(tr("Редактирование"), m_actionUndo);

    m_actionRedo = m_undoGroup->createRedoAction(this);
    m_actionRedo->setIcon(QIcon(":redo"));
    m_actionRedo->setObjectName("actionRedo");
    iMainWindow->addAction(tr("Редактирование"), m_actionRedo);

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(focusChanged(QWidget*,QWidget*)));

    connect(qApp->clipboard(), SIGNAL(dataChanged()),
            this, SLOT(clipboardDataChange()));

    m_actionCut = new QAction(QIcon(":cut"), tr("Вырезать"), this);
    m_actionCut->setObjectName("actionCut");
    connect(m_actionCut, SIGNAL(triggered()), this, SLOT(cut()));
    iMainWindow->addAction(tr("Редактирование"), m_actionCut);

    m_actionCopy = new QAction(QIcon(":copy"), tr("Копировать"), this);
    m_actionCopy->setObjectName("actionCopy");
    connect(m_actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
    iMainWindow->addAction(tr("Редактирование"), m_actionCopy);

    m_actionPaste = new QAction(QIcon(":paste"), tr("Вставить"), this);
    m_actionPaste->setObjectName("actionPaste");
    connect(m_actionPaste, SIGNAL(triggered()), this, SLOT(paste()));
    iMainWindow->addAction(tr("Редактирование"), m_actionPaste);

    m_actionSelectAll = new QAction(QIcon(":selectall"), tr("Выделить всё"), this);
    m_actionSelectAll->setObjectName("actionSelectAll");
    connect(m_actionSelectAll, SIGNAL(triggered()), this, SLOT(selectAll()));
    iMainWindow->addAction(tr("Редактирование"), m_actionSelectAll);
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
        m_actionCut->setEnabled(true);
    else
        m_actionCut->setEnabled(false);
}

void StackEditing::canCopyChange(bool canCopy)
{
    if (canCopy)
        m_actionCopy->setEnabled(true);
    else
        m_actionCopy->setEnabled(false);
}

void StackEditing::canPasteChange(bool canPaste)
{
    if (canPaste)
        m_actionPaste->setEnabled(true);
    else
        m_actionPaste->setEnabled(false);
}

void StackEditing::canSelectAllChange(bool canSelectAll)
{
    if (canSelectAll)
        m_actionSelectAll->setEnabled(true);
    else
        m_actionSelectAll->setEnabled(false);
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

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(undostack, UndoStack)
#endif



