#include "undostack.h"

#include <imainwindow.h>

using namespace RTPTechGroup::Plugin;
namespace RTPTechGroup {
namespace UndoStack {


UndoStack::UndoStack(QObject *parent):
    IUndoGroup(parent), IPlugin("IMainWindow")
{
    PluginManager* pluginManager = PluginManager::instance();

    // Создание пунктов строки меню и кнопок панели иструментов
    IMainWindow* iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    actionUndo = this->createUndoAction(this);
    actionUndo->setIcon(QIcon(":undo"));
    actionUndo->setText(tr("Отменить"));
    actionUndo->setObjectName("actionUndo");    
    iMainWindow->addAction(tr("Редактирование"),actionUndo);

    actionRedo = this->createRedoAction(this);
    actionRedo->setIcon(QIcon(":redo"));
    actionRedo->setText(tr("Повторить"));
    actionRedo->setObjectName("actionRedo");
    iMainWindow->addAction(tr("Редактирование"),actionRedo);

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(focusChanged(QWidget*,QWidget*)));
}

void UndoStack::removeStack(QUndoStack *stack)
{
    qDebug() << "test";
    foreach(QWidget *widget, m_undoStackList.keys(stack)) {
        m_undoStackList.remove(widget);
        qDebug() << widget;
    }
    QUndoGroup::removeStack(stack);
}

void UndoStack::addWidgetForStack(QUndoStack *stack, QWidget *widget)
{
    m_undoStackList[widget] = stack;
}

void UndoStack::removeWidgetForStack(QWidget *widget)
{
    m_undoStackList.remove(widget);
}

void UndoStack::focusChanged(QWidget *old, QWidget *now)
{
    Q_UNUSED(old)

    if (now != 0)
        foreach (QWidget *widget, m_undoStackList.keys())
            if (widget->isAncestorOf(now)) {
                setActiveStack(m_undoStackList.value(widget));
                return;
            }
    setActiveStack(NULL);
}

}}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(undostack, UndoStack)
#endif



