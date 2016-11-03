#include "undostack.h"

#include <imainwindow.h>

using namespace RTPTechGroup::Plugin;
namespace RTPTechGroup {
namespace StackEditing {

UndoStack::UndoStack(QObject *parent):
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
}

UndoStack::~UndoStack()
{
    m_undoStackList.clear();
    delete m_undoGroup;
}

void UndoStack::addStack(QUndoStack *stack)
{
    m_undoGroup->addStack(stack);
    connect(stack, SIGNAL(destroyed(QObject*)), this, SLOT(removeStack(QObject*)));
}

void UndoStack::removeStack(QUndoStack *stack)
{
    m_undoStackList.remove(stack);
    m_undoGroup->removeStack(stack);
}

void UndoStack::addWidgetForStack(QUndoStack *stack, QWidget *widget)
{
    m_undoStackList.insert(stack, widget);
}

void UndoStack::removeWidgetForStack(QWidget *widget)
{
    foreach (QUndoStack *undoStack, m_undoStackList.keys(widget))
        m_undoStackList.remove(undoStack, widget);
}

void UndoStack::focusChanged(QWidget *old, QWidget *now)
{
    Q_UNUSED(old)

    if (now == 0)
        return;

    setActiveStackForWidget(now);
}

void UndoStack::removeStack(QObject *obj)
{
    QUndoStack *stack = static_cast<QUndoStack *>(obj);
    removeStack(stack);
}

void UndoStack::setActiveStackForWidget(QWidget *widget)
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

}}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(undostack, UndoStack)
#endif


