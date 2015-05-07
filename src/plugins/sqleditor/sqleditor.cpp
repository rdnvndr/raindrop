#include "sqleditor.h"
#include <imainwindow.h>
#include "querysqlwidget.h"

using namespace RTPTechGroup::Plugin;
namespace RTPTechGroup {
namespace SqlEditor {


SqlEditor::SqlEditor(QObject *parent):
    QObject(parent), IPlugin("IMainWindow IUndoGroup")
{
    PluginManager* pluginManager = PluginManager::instance();

    // Создание пунктов строки меню и кнопок панели иструментов
    IMainWindow* iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    actionSqlEditor = new QAction(QIcon(":SqlEditor"), tr("SQL редактор"), this);
    connect(actionSqlEditor, SIGNAL(triggered()), this, SLOT(showSqlEditor()));
    actionSqlEditor->setObjectName("actionSqlEditor");
    iMainWindow->addAction(tr("Работа с БД"),actionSqlEditor);
}

SqlEditor::~SqlEditor()
{
    delete actionSqlEditor;
}

void SqlEditor::showSqlEditor()
{
    PluginManager *pluginManager = PluginManager::instance();
    IMainWindow *iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString subWindowName = "QuerySqlWidget";
    QMdiSubWindow* subWindow = iMainWindow->setActiveSubWindow(subWindowName);
    if (!subWindow) {
        QuerySqlWidget *queryWidget = new QuerySqlWidget(NULL);
        subWindow = iMainWindow->addSubWindow(queryWidget);
        queryWidget->setObjectName(subWindowName);
        connect(subWindow, SIGNAL(aboutToActivate()),
                queryWidget, SLOT(setActiveUndoStack()));
        subWindow->setWindowTitle(tr("SQL редактор"));
    }
}

}}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(sqleditor, SqlEditor)
#endif



