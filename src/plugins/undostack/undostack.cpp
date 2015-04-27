#include "undostack.h"

#include <imainwindow.h>

using namespace RTPTechGroup::Plugin;
namespace RTPTechGroup {
namespace Widgets {


UndoStack::UndoStack(QObject *parent):
    QObject(parent), IPlugin("IMainWindow")
{
    PluginManager* pluginManager = PluginManager::instance();

    // Создание пунктов строки меню и кнопок панели иструментов
    IMainWindow* iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    actionUndo = new QAction(QIcon(":undo"), tr("Отменить"), this);
    // connect(actionUndo, SIGNAL(triggered()), this, SLOT(undo()));
    actionUndo->setObjectName("actionUndo");
    iMainWindow->addAction(tr("Редактирование"),actionUndo);

    actionRedo = new QAction(QIcon(":redo"), tr("Повторить"), this);
    // connect(actionRedo, SIGNAL(triggered()), this, SLOT(redo()));
    actionRedo->setObjectName("actionRedo");
    iMainWindow->addAction(tr("Редактирование"),actionRedo);
}

}}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(undostack, UndoStack)
#endif



