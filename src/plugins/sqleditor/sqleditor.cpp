#include "sqleditor.h"
#include <imainwindow.h>
#include "querysqlwidget.h"

using namespace RTPTechGroup::Plugin;
namespace RTPTechGroup {
namespace SqlEditor {


SqlEditor::SqlEditor(QObject *parent):
    QObject(parent), IPlugin("IMainWindow IUndoGroup")
{
    PluginManager *pluginManager = PluginManager::instance();

    // Создание пунктов строки меню и кнопок панели иструментов
    IMainWindow *iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    m_actionSqlEditor = new QAction(QIcon(":SqlEditor"), tr("SQL редактор"), this);
    connect(m_actionSqlEditor, SIGNAL(triggered()), this, SLOT(showSqlEditor()));
    m_actionSqlEditor->setObjectName("actionSqlEditor");
    iMainWindow->addAction(tr("Работа с БД"),m_actionSqlEditor);
}

SqlEditor::~SqlEditor()
{
    delete m_actionSqlEditor;
}

void SqlEditor::showSqlEditor()
{
    PluginManager *pluginManager = PluginManager::instance();
    IMainWindow *iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString subWindowName = "QuerySqlWidget";
    QMdiSubWindow *subWindow = iMainWindow->setActiveSubWindow(subWindowName);
    if (!subWindow) {
        QuerySqlWidget *queryWidget = new QuerySqlWidget(NULL);
        subWindow = iMainWindow->addSubWindow(queryWidget);
        queryWidget->setObjectName(subWindowName);
        subWindow->setWindowTitle(tr("SQL редактор"));
    }
}

}}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(sqleditor, SqlEditor)
#endif



