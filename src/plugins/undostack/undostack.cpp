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
}

}}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(undostack, UndoStack)
#endif



