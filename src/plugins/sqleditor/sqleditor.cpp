#include "sqleditor.h"
#include <imainwindow.h>
#include "querysqlwidget.h"

using namespace RTPTechGroup::Plugin;
namespace RTPTechGroup {
namespace SqlEditor {


SqlEditor::SqlEditor(QObject *parent):
    QObject(parent), IPlugin("IMainWindow IUndoGroup IClipboardStack IDatabaseConnect")
{
    PluginManager *pluginManager = PluginManager::instance();

    // Создание пунктов строки меню и кнопок панели иструментов
    IMainWindow *iMainWindow
            = pluginManager->interfaceObject<IMainWindow*>("IMainWindow");

    m_actionSqlEditor = new QAction(QIcon(":SqlEditor"), tr("SQL редактор"), this);
    connect(m_actionSqlEditor, &QAction::triggered,
            this, &SqlEditor::showSqlEditor);
    m_actionSqlEditor->setObjectName("actionSqlEditor");
    iMainWindow->addAction(tr("Работа с БД"),m_actionSqlEditor);
}

SqlEditor::~SqlEditor()
{
    PluginManager *pluginManager = PluginManager::instance();
    IMainWindow *iMainWindow
            = pluginManager->interfaceObject<IMainWindow*>("IMainWindow");

    QMdiSubWindow *subWindow = iMainWindow->setActiveSubWindow("QuerySqlWidget");
    if (subWindow)
        subWindow->close();

    delete m_actionSqlEditor;
}

void SqlEditor::showSqlEditor()
{
    PluginManager *pluginManager = PluginManager::instance();
    IMainWindow *iMainWindow
            = pluginManager->interfaceObject<IMainWindow*>("IMainWindow");

    QString subWindowName = "QuerySqlWidget";
    QMdiSubWindow *subWindow = iMainWindow->setActiveSubWindow(subWindowName);
    if (!subWindow) {
        QuerySqlWidget *queryWidget = new QuerySqlWidget(nullptr);
        subWindow = iMainWindow->addSubWindow(queryWidget);
        queryWidget->setObjectName(subWindowName);
        subWindow->setWindowTitle(tr("SQL редактор"));
    }
}

}}
