#include <QMainWindow>

#include <treedockwidget/treedockwidget.h>

#include <mdiextarea/mdiextarea.h>
#include <treexmlmodel/tagxmlitem.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <plugin/pluginmanager.h>

#include "modelerideplug.h"
#include "propclass.h"
#include "dbxmlstruct.h"

ModelerIDEPlug::ModelerIDEPlug(IPlugin *parent):
    IPlugin(parent)
{
    setName(tr("Редактор модели данных"));
    setIcon(QIcon(":/modeleride"));
    addDepend("TreeCommandsPlug");

    dbStructModel = NULL;
}

ModelerIDEPlug::~ModelerIDEPlug()
{
    if (state.testFlag(IPlugin::Init)){
        delete classFilterModel;
        delete dbStructModel;
        delete treeClassView;
        delete actionSaveModel;
        delete actionSaveAsModel;
        delete actionNewModel;
        delete actionOpenModel;
        delete actionPublishModel;
        delete actionCloseModel;
        delete actionAddClass;
        delete actionRemoveClass;
        delete contextMenu;
    }
}

bool ModelerIDEPlug::initialize(){

    PluginManager* pluginManager = PluginManager::instance();
    TreeDockWidget* dockWidget = static_cast<TreeDockWidget*>(pluginManager->getObjectByName(
                                                                   "TreeCommandsPlug::TreeDockWidget"));
    treeClassView = new ClassTreeView();

    connect(treeClassView->treeView,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(showPropClass(QModelIndex)));


    // Создание контекстного меню
    contextMenu = new QMenu();
    actionAddClass = new QAction(tr("Добавить"),this);
    contextMenu->addAction(actionAddClass);
    connect(actionAddClass,SIGNAL(triggered()),this,SLOT(addClass()));
    actionRemoveClass = new QAction(tr("Удалить"),this);
    contextMenu->addAction(actionRemoveClass);
    connect(actionRemoveClass,SIGNAL(triggered()),this,SLOT(removeClass()));
    treeClassView->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeClassView->treeView,SIGNAL(customContextMenuRequested(const QPoint&)),
            this,SLOT(showContextMenu(const QPoint&)));

    // Создание пунктов строки меню и кнопок панели исрументов
    QObject *object = pluginManager->getObjectByName("MainWindowPlug::MainWindow");
    QMainWindow* mainWindow = static_cast<QMainWindow*>(object);

    object = pluginManager->getObjectByName("MainWindowPlug::menuFile");
    QMenu* menuFile = static_cast<QMenu*>(object);

    QAction* separator;

    actionNewModel = new QAction(QIcon(":newmodel"), tr("Новая модель"), mainWindow);
    connect(actionNewModel, SIGNAL(triggered()), this, SLOT(newClassModel()));

    actionOpenModel = new QAction(QIcon(":openmodel"), tr("Открыть модель..."), mainWindow);
    connect(actionOpenModel, SIGNAL(triggered()), this, SLOT(openClassModel()));

    actionSaveModel = new QAction(QIcon(":savemodel"), tr("Сохранить модель"), mainWindow);
    connect(actionSaveModel, SIGNAL(triggered()), this, SLOT(saveClassModel()));
    actionSaveModel->setDisabled(true);

    actionSaveAsModel = new QAction(QIcon(":savemodel"), tr("Сохранить модель как..."), mainWindow);
    connect(actionSaveAsModel, SIGNAL(triggered()), this, SLOT(saveAsClassModel()));
    actionSaveAsModel->setDisabled(true);

    actionPublishModel = new QAction(QIcon(":publish"), tr("Опубликовать модель..."), mainWindow);
    connect(actionPublishModel, SIGNAL(triggered()), this, SLOT(publishClassModel()));
    actionPublishModel->setDisabled(true);

    actionCloseModel = new QAction(QIcon(":closemodel"), tr("Закрыть модель"), mainWindow);
    connect(actionCloseModel, SIGNAL(triggered()), this, SLOT(closeClassModel()));
    actionCloseModel->setDisabled(true);

    if (menuFile->actions().count() > 0)
        separator = menuFile->insertSeparator(menuFile->actions().first());
    else
        separator = menuFile->addSeparator();

    menuFile->insertAction(separator, actionNewModel);
    menuFile->insertAction(separator, actionOpenModel);
    menuFile->insertAction(separator, actionSaveModel);
    menuFile->insertAction(separator, actionSaveAsModel);
    menuFile->insertAction(separator, actionPublishModel);
    menuFile->insertAction(separator, actionCloseModel);

    object = pluginManager->getObjectByName("MainWindowPlug::toolBarMain");
    QToolBar* toolBarMain = static_cast<QToolBar*>(object);

    if (toolBarMain->actions().count() > 0)
        separator = toolBarMain->insertSeparator(toolBarMain->actions().first());
    else
        separator = toolBarMain->addSeparator();

    toolBarMain->insertAction(separator, actionNewModel);
    toolBarMain->insertAction(separator, actionOpenModel);
    toolBarMain->insertAction(separator, actionSaveModel);
    toolBarMain->insertAction(separator, actionPublishModel);
    toolBarMain->insertAction(separator, actionCloseModel);

    dockWidget->insertWidget(QIcon(":/modeleride"),tr("Редактор модели данных"),treeClassView);
    return true;
}

bool ModelerIDEPlug::release()
{
    return true;
}

void ModelerIDEPlug::showContextMenu(const QPoint& point)
{
    if (dbStructModel){
            contextMenu->exec(treeClassView->treeView->mapToGlobal(point));
    }
    return;
}

TreeXMLModel *ModelerIDEPlug::model()
{
    return dbStructModel;
}

void ModelerIDEPlug::createClassModel(QDomDocument document)
{
    dbStructModel = new TreeXMLModel(document, this);

    dbStructModel->addTagFilter(DBCLASSXML::CLASS);
    dbStructModel->addTagFilter(DBATTRXML::ATTR);

    QStringList propsClass;
    propsClass << DBCLASSXML::NAME << DBCLASSXML::ISABSTARCT <<
                  DBCLASSXML::TYPE << DBCLASSXML::DESCRIPTION <<
                  DBCLASSXML::PARENT << DBCLASSXML::ISACTIVE <<
                  DBCLASSXML::TEMPLATE;
    dbStructModel->addDisplayedAttr(DBCLASSXML::CLASS,propsClass,QIcon(":/modeleride"));
    dbStructModel->addDisplayedAttr(DBATTRXML::ATTR,propsClass,QIcon(":/attribute"));

    QStringList propsAttr;
    propsAttr << DBATTRXML::NAME << DBATTRXML::DESCRIPTION << DBATTRXML::TYPE <<
                 DBATTRXML::MAXSTRLEN << DBATTRXML::REFCLASS <<
                 DBATTRXML::PARENT << DBATTRXML::INITIALVAL <<
                 DBATTRXML::GROUP << DBATTRXML::ISNULLALLOWED <<
                 DBATTRXML::ISUNIQUE << DBATTRXML::ISCANDIDATEKEY;

    dbStructModel->setHeaderData(0, Qt::Horizontal, tr("Имя атирибута"));
    dbStructModel->setHeaderData(1, Qt::Horizontal, tr("Описание"));
    dbStructModel->setHeaderData(2, Qt::Horizontal, tr("Тип"));
    dbStructModel->setHeaderData(3, Qt::Horizontal, tr("Длина строки"));
    dbStructModel->setHeaderData(4, Qt::Horizontal, tr("Ссылочный класс"));
    dbStructModel->setHeaderData(5, Qt::Horizontal, tr("Класс"));
    dbStructModel->setHeaderData(6, Qt::Horizontal, tr("По умолчанию"));
    dbStructModel->setHeaderData(7, Qt::Horizontal, tr("Группа"));
    dbStructModel->setHeaderData(8, Qt::Horizontal, tr("Нулевые значения"));
    dbStructModel->setHeaderData(9, Qt::Horizontal, tr("Уникальный"));
    dbStructModel->setHeaderData(10, Qt::Horizontal, tr("Кандидат в ключ"));

    dbStructModel->addDisplayedAttr(DBATTRXML::ATTR,propsAttr);
    dbStructModel->addAttributeTag(DBATTRXML::ATTR);

    classFilterModel = new TreeFilterProxyModel();
    classFilterModel->setSourceModel(dbStructModel);
    classFilterModel->setDynamicSortFilter(true);

    connect(treeClassView->lineEditFiler,SIGNAL(textChanged(QString)),
            classFilterModel,SLOT(setFilterRegExp(QString)));

    treeClassView->treeView->setModel(classFilterModel);
    for (int i=1;i<dbStructModel->columnCount();i++)
        treeClassView->treeView->hideColumn(i);

    actionPublishModel->setEnabled(true);
    actionSaveModel->setEnabled(true);
    actionSaveAsModel->setEnabled(true);
    actionCloseModel->setEnabled(true);
}

void ModelerIDEPlug::addClass()
{
    dbStructModel->setInsTagName(DBCLASSXML::CLASS);
    QModelIndex index = classFilterModel->mapToSource(treeClassView->treeView->currentIndex());
    if (dbStructModel->insertRow(0,index)){
        index = classFilterModel->mapFromSource(dbStructModel->lastInsertRow());
        treeClassView->treeView->setCurrentIndex(index);
        showPropClass(index);
    }
}

QString ModelerIDEPlug::className(const QModelIndex& index)
{
    return index.sibling(index.row(),dbStructModel->indexDisplayedAttr(
                             DBCLASSXML::CLASS,
                             DBCLASSXML::NAME
                             )).data().toString();
}

void ModelerIDEPlug::removeClass()
{
    QModelIndex currentIndex = classFilterModel->mapToSource(
                treeClassView->treeView->currentIndex());
    if (currentIndex.isValid()){
        if (!className(currentIndex).isEmpty())
        {
            PluginManager* pluginManager = PluginManager::instance();
            MainWindow* mainwindow = static_cast<MainWindow*>(pluginManager->getObjectByName(
                                                                  "MainWindowPlug::MainWindow"));

            QString subWindowName = "PropClass::" + this->className(currentIndex);
            QMdiSubWindow *subWindow = mainwindow->subWindow(subWindowName);
            if (subWindow)
                subWindow->close();
            dbStructModel->removeRow(currentIndex.row(),currentIndex.parent());
        } else
            QMessageBox::warning(NULL,tr("Предупреждение"),
                                 tr("Невозможно удалить узел, поскольку он еще не сохранен."));
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить узел, поскольку он не выбран."));
}

void ModelerIDEPlug::showPropClass(QModelIndex index)
{
    QModelIndex indexSource = classFilterModel->mapToSource(index);

    if (!indexSource.isValid())
        return;

    if (dbStructModel->isAttribute(indexSource))
        return;

    PluginManager* pluginManager = PluginManager::instance();
    MainWindow* mainwindow = static_cast<MainWindow*>(pluginManager->getObjectByName(
                                                           "MainWindowPlug::MainWindow"));

    QString subWindowName = "PropClass::" + this->className(index);
    QMdiSubWindow* subWindow = mainwindow->setActiveSubWindow(subWindowName);

    if (!subWindow) {
        PropClass* propClass = new PropClass();
        subWindow =  mainwindow->addSubWindow(propClass);
        propClass->setObjectName(subWindowName);
        propClass->setModel(dbStructModel);
        propClass->setCurrentClass(indexSource);
    } else {
        PropClass* propClass = qobject_cast<PropClass*>(subWindow->widget());
        propClass->setCurrentClass(indexSource);
    }
}

void ModelerIDEPlug::newClassModel()
{
    closeClassModel();
    QDomDocument document;
    QDomNode node = document.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    document.insertBefore(node, document.firstChild());
    createClassModel(document);
}

void ModelerIDEPlug::openClassModel()
{
    fileName = QFileDialog::getOpenFileName(NULL, tr("Открытие модели"),
                                                    "", tr("Фаил модели (*.xml)"));
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QDomDocument document;
        if (document.setContent(&file)) {
           createClassModel(document);
        }
        file.close();
    }
}

void ModelerIDEPlug::saveClassModel()
{
    if (fileName.isEmpty())
        saveAsClassModel();

    QFile File(fileName);
    if ( File.open(QIODevice::WriteOnly) )
    {
        QTextStream TextStream(&File);
        QDomDocument doc = dbStructModel->rootItem()->node().toDocument();
        doc.save(TextStream, 0);
        File.close();
    }
}

void ModelerIDEPlug::saveAsClassModel()
{
    fileName = QFileDialog::getSaveFileName(NULL, tr("Сохранение модели"),
                                                    "", tr("Фаил модели (*.xml)"));
    QFile File(fileName);
    if ( File.open(QIODevice::WriteOnly) )
    {
        QTextStream TextStream(&File);
        QDomDocument doc = dbStructModel->rootItem()->node().toDocument();
        doc.save(TextStream, 0);
        File.close();
    }
}

void ModelerIDEPlug::publishClassModel()
{

}

void ModelerIDEPlug::closeClassModel()
{
    if (dbStructModel){
        delete dbStructModel;
        dbStructModel = NULL;
        actionCloseModel->setDisabled(true);
        actionSaveModel->setDisabled(true);
        actionSaveAsModel->setDisabled(true);
        actionPublishModel->setDisabled(true);
    }
}

Q_EXPORT_PLUGIN2(modeleride, ModelerIDEPlug)
