#include "stackediting.h"

#include <imainwindow.h>

using namespace RTPTechGroup::Plugin;
namespace RTPTechGroup {
namespace StackEditing {


StackEditing::StackEditing(QObject *parent):
    QObject(parent), IPlugin("IMainWindow")
{
    m_undoGroup = new QUndoGroup(this);

    PluginManager* pluginManager = PluginManager::instance();

    // Создание пунктов строки меню и кнопок панели иструментов
    IMainWindow* iMainWindow = qobject_cast<IMainWindow*>(
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
}

StackEditing::~StackEditing()
{
    m_undoStackList.clear();
    delete m_undoGroup;
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

    QMutableMapIterator<QUndoStack *, QWidget *> i(m_undoStackList);
    i.toBack();
    while (i.hasPrevious()) {
        i.previous();
        if (i.value()->isAncestorOf(now)) {
            m_undoGroup->setActiveStack(i.key());
            return;
        }
    }
    m_undoGroup->setActiveStack(NULL);
}

void StackEditing::removeStack(QObject *obj)
{
    QUndoStack *stack = static_cast<QUndoStack *>(obj);
    removeStack(stack);
}

}}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(undostack, UndoStack)
#endif



