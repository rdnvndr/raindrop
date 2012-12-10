#include <QMainWindow>

#include <treedockwidget/treedockwidget.h>

#include <mdiextarea/mdiextarea.h>
#include <treexmlmodel/tagxmlitem.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <plugin/pluginmanager.h>

#include "modelerideplug.h"
#include "propclass.h"
#include "propcomposition.h"
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
        delete treeClassView;
        closeClassModel();
        delete actionSaveModel;
        delete actionSaveAsModel;
        delete actionNewModel;
        delete actionOpenModel;
        delete actionPublishModel;
        delete actionCloseModel;
        delete actionAddClass;
        delete actionRemoveClass;
        delete actionShowAttr;
        delete actionShowComp;
        delete actionSeparator;
        delete contextMenu;
    }
}

bool ModelerIDEPlug::initialize(){

    PluginManager* pluginManager = PluginManager::instance();
    TreeDockWidget* dockWidget = static_cast<TreeDockWidget*>(pluginManager->getObjectByName(
                                                                   "TreeCommandsPlug::TreeDockWidget"));
    treeClassView = new ClassTreeView();

    connect(treeClassView->treeView,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(dblClickTree(QModelIndex)));

    // Создание контекстного меню
    contextMenu = new QMenu();

    actionAddClass = new QAction(tr("Добавить"),this);
    contextMenu->addAction(actionAddClass);
    connect(actionAddClass,SIGNAL(triggered()),this,SLOT(addClass()));

    actionRemoveClass = new QAction(tr("Удалить"),this);
    contextMenu->addAction(actionRemoveClass);
    connect(actionRemoveClass,SIGNAL(triggered()),this,SLOT(removeClass()));

    actionSeparator = new QAction(tr("Разделитель"),this);
    actionSeparator->setSeparator(true);
    contextMenu->addAction(actionSeparator);

    actionShowAttr = new QAction(tr("Показать атрибуты"),this);
    actionShowAttr->setCheckable(true);
    contextMenu->addAction(actionShowAttr);
    connect(actionShowAttr,SIGNAL(triggered(bool)),this,SLOT(setShownAttr(bool)));

    actionShowComp = new QAction(tr("Показать состав"),this);
    actionShowComp->setCheckable(true);
    contextMenu->addAction(actionShowComp);
    connect(actionShowComp,SIGNAL(triggered(bool)),this,SLOT(setShownComp(bool)));

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

void ModelerIDEPlug::setShownAttr(bool shown)
{
    if (shown)
        classFilterModel->removeHiddenTag(DBATTRXML::ATTR);
    else
        classFilterModel->addHiddenTag(DBATTRXML::ATTR);

    QRegExp regex = classFilterModel->filterRegExp();
    classFilterModel->setFilterRegExp(regex);
    actionShowAttr->setChecked(shown);
}

void ModelerIDEPlug::setShownComp(bool shown)
{
    if (shown)
        classFilterModel->removeHiddenTag(DBCOMPXML::COMP);
    else
        classFilterModel->addHiddenTag(DBCOMPXML::COMP);

    QRegExp regex = classFilterModel->filterRegExp();
    classFilterModel->setFilterRegExp(regex);
    actionShowComp->setChecked(shown);
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
    dbStructModel->addTagFilter(DBCOMPXML::COMP);

    QStringList propsClass;
    propsClass << DBCLASSXML::NAME << DBCLASSXML::ISABSTARCT <<
                  DBCLASSXML::TYPE << DBCLASSXML::DESCRIPTION <<
                  DBCLASSXML::PARENT << DBCLASSXML::ISACTIVE <<
                  DBCLASSXML::TEMPLATE << DBCLASSXML::ID;
    dbStructModel->addDisplayedAttr(DBCLASSXML::CLASS,propsClass,QIcon(":/modeleride"));
    dbStructModel->addDisplayedAttr(DBATTRXML::ATTR,propsClass,QIcon(":/attribute"));
    dbStructModel->addDisplayedAttr(DBCOMPXML::COMP,propsClass,QIcon(":/composition"));

    QStringList propsAttr;
    propsAttr << DBATTRXML::NAME << DBATTRXML::DESCRIPTION << DBATTRXML::TYPE <<
                 DBATTRXML::MAXSTRLEN << DBATTRXML::REFCLASS <<
                 DBATTRXML::PARENT << DBATTRXML::INITIALVAL <<
                 DBATTRXML::GROUP << DBATTRXML::ISNULLALLOWED <<
                 DBATTRXML::ISUNIQUE << DBATTRXML::ISCANDIDATEKEY <<
                 DBATTRXML::ID;

    QStringList propsComposition;
    propsComposition << DBCOMPXML::NAME << DBCOMPXML::DESCRIPTION <<
                 DBCOMPXML::PARENT << DBCOMPXML::CLASS <<
                 DBCOMPXML::DIRECTDESCRIPTION << DBCOMPXML::INVERSEDESCRIPTION <<
                 DBCOMPXML::ID;

    dbStructModel->setHeaderData(0, Qt::Horizontal, tr("Имя атрибута"));
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

    dbStructModel->addDisplayedAttr(DBCOMPXML::COMP,propsComposition);
    dbStructModel->addAttributeTag(DBCOMPXML::COMP);

    QStringList insertTags;
    insertTags << DBATTRXML::ATTR << DBCLASSXML::CLASS << DBCOMPXML::COMP;
    dbStructModel->addInsertTags(DBCLASSXML::CLASS,insertTags);
    insertTags.clear();
    insertTags << DBATTRXML::ATTR;
    dbStructModel->addInsertTags(DBCOMPXML::COMP,insertTags);

    classFilterModel = new TreeFilterProxyModel();
    classFilterModel->setSourceModel(dbStructModel);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);

    setShownComp(false);   
    setShownAttr(false);

    connect(treeClassView->lineEditFiler,SIGNAL(textChanged(QString)),
            classFilterModel,SLOT(setFilterRegExp(QString)));

    connect(treeClassView->lineEditFiler,SIGNAL(textChanged(QString)),
            treeClassView->treeView,SLOT(expandAll()));

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
    QModelIndex indexSource = classFilterModel->mapToSource(treeClassView->treeView->currentIndex());
    if (dbStructModel->insertRow(0,indexSource)){
        QModelIndex index = classFilterModel->mapFromSource(dbStructModel->lastInsertRow());
        treeClassView->treeView->setCurrentIndex(index);
        showPropClass(dbStructModel->lastInsertRow());
    }
}

QString ModelerIDEPlug::className(const QModelIndex& index)
{
    return index.sibling(index.row(),dbStructModel->indexDisplayedAttr(
                             DBCLASSXML::CLASS,
                             DBCLASSXML::NAME
                             )).data().toString();
}

void ModelerIDEPlug::dblClickTree(QModelIndex index)
{
    QModelIndex indexSource = classFilterModel->mapToSource(index);

    if (!indexSource.isValid())
        return;

    if (!dbStructModel->isAttribute(indexSource))
        showPropClass(indexSource);

    if (indexSource.data(Qt::UserRole)==DBCOMPXML::COMP)
        showPropComposition(indexSource);
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

void ModelerIDEPlug::showPropClass(QModelIndex indexSource)
{
    if (!indexSource.isValid())
        return;

    if (dbStructModel->isAttribute(indexSource))
        return;

    PluginManager* pluginManager = PluginManager::instance();
    MainWindow* mainwindow = static_cast<MainWindow*>(pluginManager->getObjectByName(
                                                           "MainWindowPlug::MainWindow"));

    QString subWindowName = "PropClass::" + this->className(indexSource);
    QMdiSubWindow* subWindow = mainwindow->setActiveSubWindow(subWindowName);

    if (!subWindow) {
        PropClass* propClass = new PropClass();
        subWindow =  mainwindow->addSubWindow(propClass);
        propClass->setObjectName(subWindowName);
        propClass->setModel(dbStructModel);
        propClass->setCurrentClass(indexSource);
        connect(propClass,SIGNAL(editComposition(QModelIndex)),
                this,SLOT(showPropComposition(QModelIndex)));
    } else {
        PropClass* propClass = qobject_cast<PropClass*>(subWindow->widget());
        propClass->setCurrentClass(indexSource);
    }
}

void ModelerIDEPlug::showPropComposition(QModelIndex indexSource)
{
    if (!indexSource.isValid())
        return;

    if (indexSource.data(Qt::UserRole)!=DBCOMPXML::COMP)
        return;

    PluginManager* pluginManager = PluginManager::instance();
    MainWindow* mainwindow = static_cast<MainWindow*>(pluginManager->getObjectByName(
                                                           "MainWindowPlug::MainWindow"));

    QString className = indexSource.sibling(indexSource.row(),dbStructModel->indexDisplayedAttr(
                                 DBCOMPXML::COMP,
                                 DBCOMPXML::NAME
                                 )).data().toString();

    QString subWindowName = "PropComposition::" + className;
    QMdiSubWindow* subWindow = mainwindow->setActiveSubWindow(subWindowName);

    if (!subWindow) {
        PropComposition* propComposition = new PropComposition();
        subWindow =  mainwindow->addSubWindow(propComposition);
        propComposition->setObjectName(subWindowName);
        propComposition->setModel(dbStructModel);
        propComposition->setCurrentClass(indexSource);
    } else {
        PropComposition* propComposition = qobject_cast<PropComposition*>(subWindow->widget());
        propComposition->setCurrentClass(indexSource);
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

void ModelerIDEPlug::publishClassModel(QModelIndex index)
{
    for (int row=0;row < dbStructModel->rowCount(index);row++){
        QModelIndex childIndex = dbStructModel->index(row,0,index);

        if (childIndex.data(Qt::UserRole)==DBATTRXML::ATTR){
            // Создание атрибута
            if (dbStructModel->isInherited(childIndex))
                qDebug() << "Унаследованный атрибут:" << childIndex.data().toString();
            else
                qDebug() << "Атрибут:" << childIndex.data().toString();
        } else if (childIndex.data(Qt::UserRole)==DBCOMPXML::COMP) {
            // Создание состава
            if (dbStructModel->isInherited(childIndex))
                qDebug() << "Унаследованный состав:" <<childIndex.data().toString();
            else
                qDebug() << "Состав:" <<childIndex.data().toString();
            publishClassModel(childIndex);
        } else if (childIndex.data(Qt::UserRole)==DBCLASSXML::CLASS) {
            // Создание класса
            qDebug() << "Класс:" << childIndex.data().toString();
            publishClassModel(childIndex);
        }
    }
}

void ModelerIDEPlug::closeClassModel()
{
    PluginManager* pluginManager = PluginManager::instance();
    MainWindow* mainwindow = static_cast<MainWindow*>(pluginManager->getObjectByName(
                                                           "MainWindowPlug::MainWindow"));
    foreach (QMdiSubWindow* subWindow, mainwindow->subWindowList())
        if (subWindow->widget()->objectName().indexOf(QRegExp("^PropClass::"))
                || subWindow->widget()->objectName().indexOf(QRegExp("^PropComposition::")))
            subWindow->close();

    if (dbStructModel){
        delete classFilterModel;
        classFilterModel = NULL;
        delete dbStructModel;
        dbStructModel = NULL;
        actionCloseModel->setDisabled(true);
        actionSaveModel->setDisabled(true);
        actionSaveAsModel->setDisabled(true);
        actionPublishModel->setDisabled(true);
    }
}

Q_EXPORT_PLUGIN2(modeleride, ModelerIDEPlug)
