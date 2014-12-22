#include <QMainWindow>
#include <QFileDialog>
#include <QMenu>

#include <itreedockwidget.h>
#include <imainwindow.h>

#include <mdiextarea/mdiextarea.h>
#include <treexmlmodel/tagxmlitem.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <plugin/pluginmanager.h>

#include "modelerideplug.h"
#include "propclass.h"
#include "propfilter.h"
#include "propentity.h"
#include "dbxmlstruct.h"
#include "propentitygroup.h"
#include "lovwidget.h"
#include "proplov.h"
#include "proprefgroup.h"
#include "propref.h"

ModelerIDEPlug::ModelerIDEPlug(QObject *parent):
    QObject(parent), IPlugin("ITreeDockWidget IMainWindow")
{
    m_model = NULL;

    PluginManager* pluginManager = PluginManager::instance();

    treeClassView = new ClassTreeView();
    connect(treeClassView,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(dblClickTree(QModelIndex)));
    connect(treeClassView,SIGNAL(actionInserted()),
            this,SLOT(add()));
    connect(treeClassView,SIGNAL(actionRemoved()),
            this,SLOT(remove()));

    // Создание пунктов строки меню и кнопок панели исрументов
    IMainWindow* iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    actionNewModel = new QAction(QIcon(":newmodel"), tr("Новая модель"), this);
    connect(actionNewModel, SIGNAL(triggered()), this, SLOT(newClassModel()));
    actionNewModel->setObjectName("actionNewModel");
    iMainWindow->addAction(tr("Редактор модели"),actionNewModel);

    actionOpenModel = new QAction(QIcon(":openmodel"), tr("Открыть модель..."), this);
    connect(actionOpenModel, SIGNAL(triggered()), this, SLOT(openClassModel()));
    actionOpenModel->setObjectName("actionOpenModel");
    iMainWindow->addAction(tr("Редактор модели"),actionOpenModel);

    actionSaveModel = new QAction(QIcon(":savemodel"), tr("Сохранить модель"), this);
    connect(actionSaveModel, SIGNAL(triggered()), this, SLOT(saveClassModel()));
    actionSaveModel->setDisabled(true);
    actionSaveModel->setObjectName("actionSaveModel");
    iMainWindow->addAction(tr("Редактор модели"),actionSaveModel);

    actionSaveAsModel = new QAction(QIcon(":savemodel"), tr("Сохранить модель как..."), this);
    connect(actionSaveAsModel, SIGNAL(triggered()), this, SLOT(saveAsClassModel()));
    actionSaveAsModel->setDisabled(true);
    actionSaveAsModel->setObjectName("actionSaveAsModel");
    iMainWindow->addAction(tr("Редактор модели"),actionSaveAsModel);

    actionPublishModel = new QAction(QIcon(":publish"), tr("Опубликовать модель..."), this);
    connect(actionPublishModel, SIGNAL(triggered()), this, SLOT(publishClassModel()));
    actionPublishModel->setDisabled(true);
    actionPublishModel->setObjectName("actionPublishModel");
    iMainWindow->addAction(tr("Редактор модели"),actionPublishModel);

    actionCloseModel = new QAction(QIcon(":closemodel"), tr("Закрыть модель"), this);
    connect(actionCloseModel, SIGNAL(triggered()), this, SLOT(closeClassModel()));
    actionCloseModel->setDisabled(true);
    actionCloseModel->setObjectName("actionCloseModel");
    iMainWindow->addAction(tr("Редактор модели"),actionCloseModel);

    dockWidget = new DockWidget();
    dockWidget->setObjectName("MetamodelDockWidget");
    dockWidget->setWidget(treeClassView);
    dockWidget->setWindowTitle(tr("Модель метаданных"));
    iMainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
}

ModelerIDEPlug::~ModelerIDEPlug()
{
    delete treeClassView;
    delete dockWidget;
    closeClassModel();
    delete actionSaveModel;
    delete actionSaveAsModel;
    delete actionNewModel;
    delete actionOpenModel;
    delete actionPublishModel;
    delete actionCloseModel;
}

void ModelerIDEPlug::actionSaveEnable()
{
    actionSaveModel->setEnabled(true);
}

TreeXmlHashModel *ModelerIDEPlug::model()
{
    return m_model;
}

void ModelerIDEPlug::createClassModel(QDomDocument document)
{
    m_model = new ClassModel(document, this);

    m_model->setHeaderData(0,  Qt::Horizontal, tr("Имя атрибута"));
    m_model->setHeaderData(1,  Qt::Horizontal, tr("Псевдоним"));
    m_model->setHeaderData(2,  Qt::Horizontal, tr("Тип"));
    m_model->setHeaderData(3,  Qt::Horizontal, tr("Длина строки"));
    m_model->setHeaderData(4,  Qt::Horizontal, tr("Ссылочный класс"));
    m_model->setHeaderData(5,  Qt::Horizontal, tr("Класс"));
    m_model->setHeaderData(6,  Qt::Horizontal, tr("ЕИ"));
    m_model->setHeaderData(7,  Qt::Horizontal, tr("По умолчанию"));
    m_model->setHeaderData(8,  Qt::Horizontal, tr("Нижняя граница"));
    m_model->setHeaderData(9,  Qt::Horizontal, tr("Верхняя гранница"));
    m_model->setHeaderData(10, Qt::Horizontal, tr("Список значений"));
    m_model->setHeaderData(11, Qt::Horizontal, tr("Группа"));
    m_model->setHeaderData(12, Qt::Horizontal, tr("Нулевые значения"));
    m_model->setHeaderData(13, Qt::Horizontal, tr("Уникальный"));
    m_model->setHeaderData(14, Qt::Horizontal, tr("Кандидат в ключ"));
    m_model->setHeaderData(15, Qt::Horizontal, tr("Индетификатор"));

    connect(m_model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this,SLOT(actionSaveEnable()));
    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(actionSaveEnable()));

    treeClassView->setModel(m_model);

    actionPublishModel->setEnabled(true);
    actionSaveModel->setEnabled(true);
    actionSaveAsModel->setEnabled(true);
    actionCloseModel->setEnabled(true);
}

void ModelerIDEPlug::add()
{
    QModelIndex indexSource = treeClassView->currentIndex();
    if (!indexSource.isValid())
        return;

    if (indexSource.data(TreeXmlModel::TagRole)==DBCLASSLISTXML::CLASSLIST
            || indexSource.data(TreeXmlModel::TagRole)==DBCLASSXML::CLASS)
    {
        QModelIndex lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBCLASSXML::CLASS);
        if (lastInsertRow.isValid()){
            int column = m_model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                      DBCLASSXML::TYPE);
            m_model->setData(lastInsertRow.sibling(lastInsertRow.row(),column),
                             DBCLASSTYPEXML::STANDART);

            treeClassView->setCurrentIndex(lastInsertRow);
            showPropClass(lastInsertRow);
        }
    } else if (indexSource.data(TreeXmlModel::TagRole)==DBENTITYLISTXML::ENTITYLIST) {
        QModelIndex lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBENTITYGROUPXML::ENTITYGROUP);
        if (lastInsertRow.isValid()){
            treeClassView->setCurrentIndex(lastInsertRow);
            showPropEntityGroup(lastInsertRow);
        }
    } else if (indexSource.data(TreeXmlModel::TagRole)==DBENTITYGROUPXML::ENTITYGROUP) {
        QModelIndex lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBENTITYXML::ENTITY);
        if (lastInsertRow.isValid()){
            treeClassView->setCurrentIndex(lastInsertRow);
            showPropEntity(lastInsertRow);
        }
    } else if (indexSource.data(TreeXmlModel::TagRole)==DBLOVLISTXML::LOVLIST) {
        QModelIndex lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBLOVXML::LOV);
        if (lastInsertRow.isValid()){
            treeClassView->setCurrentIndex(lastInsertRow);
            showPropLov(lastInsertRow);
        }
    } else if (indexSource.data(TreeXmlModel::TagRole)==DBREFLISTXML::REFLIST) {
        QModelIndex lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBREFGROUPXML::REFGROUP);
        if (lastInsertRow.isValid()){
            treeClassView->setCurrentIndex(lastInsertRow);
            showPropRefGroup(lastInsertRow);
        }
    } else if (indexSource.data(TreeXmlModel::TagRole)==DBREFGROUPXML::REFGROUP) {
        QModelIndex lastInsertRow =
                m_model->insertLastRows(0,1,indexSource,DBREFXML::REF);
        if (lastInsertRow.isValid()){
            treeClassView->setCurrentIndex(lastInsertRow);
            showPropRef(lastInsertRow);
        }
    }
}

QString ModelerIDEPlug::dataName(const QModelIndex& index)
{
    if (index.data(TreeXmlModel::TagRole) == DBCLASSXML::CLASS)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBCLASSXML::CLASS,
                                 DBCLASSXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBENTITYXML::ENTITY)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBENTITYXML::ENTITY,
                                 DBENTITYXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBCOMPXML::COMP)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBCOMPXML::COMP,
                                 DBCOMPXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBFILTERXML::FILTER)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBFILTERXML::FILTER,
                                 DBFILTERXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBENTITYGROUPXML::ENTITYGROUP) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBENTITYGROUPXML::ENTITYGROUP,
                                 DBENTITYGROUPXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBLOVXML::LOV) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBLOVXML::LOV,
                                 DBLOVXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBREFGROUPXML::REFGROUP) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBREFGROUPXML::REFGROUP,
                                 DBREFGROUPXML::NAME
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBREFXML::REF)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBREFXML::REF,
                                 DBREFXML::NAME
                                 )).data().toString();
    }

    return "";
}

QString ModelerIDEPlug::dataId(const QModelIndex &index)
{
    if (index.data(TreeXmlModel::TagRole) == DBCLASSXML::CLASS)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBCLASSXML::CLASS,
                                 DBCLASSXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBENTITYXML::ENTITY)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBENTITYXML::ENTITY,
                                 DBENTITYXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBCOMPXML::COMP)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBCOMPXML::COMP,
                                 DBCOMPXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBFILTERXML::FILTER)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBFILTERXML::FILTER,
                                 DBFILTERXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBENTITYGROUPXML::ENTITYGROUP) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBENTITYGROUPXML::ENTITYGROUP,
                                 DBENTITYGROUPXML::ID
                                 )).data().toString();
    }


    if (index.data(TreeXmlModel::TagRole) == DBLOVXML::LOV) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBLOVXML::LOV,
                                 DBLOVXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBREFGROUPXML::REFGROUP) {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBREFGROUPXML::REFGROUP,
                                 DBREFGROUPXML::ID
                                 )).data().toString();
    }

    if (index.data(TreeXmlModel::TagRole) == DBREFXML::REF)
    {
        return index.sibling(index.row(),m_model->columnDisplayedAttr(
                                 DBREFXML::REF,
                                 DBREFXML::ID
                                 )).data().toString();
    }
    return "";
}

void ModelerIDEPlug::dblClickTree(const QModelIndex &index)
{
    QModelIndex indexSource = index;

    if (!indexSource.isValid())
        return;

    if (indexSource.data(TreeXmlModel::TagRole)==DBCLASSXML::CLASS)
        showPropClass(indexSource);

    if (indexSource.data(TreeXmlModel::TagRole)==DBFILTERXML::FILTER)
        showPropFilter(indexSource);

    if (indexSource.data(TreeXmlModel::TagRole)==DBENTITYXML::ENTITY)
        showPropEntity(indexSource);

    if (indexSource.data(TreeXmlModel::TagRole)==DBENTITYGROUPXML::ENTITYGROUP)
        showPropEntityGroup(indexSource);

    if (indexSource.data(TreeXmlModel::TagRole)==DBLOVXML::LOV)
        showPropLov(indexSource);

    if (indexSource.data(TreeXmlModel::TagRole)==DBREFGROUPXML::REFGROUP)
        showPropRefGroup(indexSource);

    if (indexSource.data(TreeXmlModel::TagRole)==DBREFXML::REF)
        showPropRef(indexSource);
}

bool ModelerIDEPlug::isRemove(const QModelIndex &srcIndex)
{
    if (srcIndex.data(TreeXmlModel::TagRole) == DBMODELXML::MODEL
            || srcIndex.data(TreeXmlModel::TagRole) == DBCLASSLISTXML::CLASSLIST
            || srcIndex.data(TreeXmlModel::TagRole) == DBENTITYLISTXML::ENTITYLIST
            || srcIndex.data(TreeXmlModel::TagRole) == DBLOVLISTXML::LOVLIST)
    {
        return false;
    }

    if (srcIndex.data(TreeXmlModel::TagRole) == DBENTITYGROUPXML::ENTITYGROUP)
        return PropEntityGroup::isRemove(srcIndex);

    if (srcIndex.data(TreeXmlModel::TagRole) == DBENTITYXML::ENTITY)
        return MsrEntityWidget::isRemove(srcIndex);

    if (srcIndex.data(TreeXmlModel::TagRole) == DBUNITXML::UNIT)
        return MsrUnitWidget::isRemove(srcIndex);

    if (srcIndex.data(TreeXmlModel::TagRole) == DBCLASSXML::CLASS)
        return ClassWidget::isRemove(srcIndex);

    if (srcIndex.data(TreeXmlModel::TagRole) == DBLOVXML::LOV)
        return LovWidget::isRemove(srcIndex);

    if (srcIndex.data(TreeXmlModel::TagRole) == DBREFGROUPXML::REFGROUP)
        return PropRefGroup::isRemove(srcIndex);

    if (srcIndex.data(TreeXmlModel::TagRole) == DBREFXML::REF)
        return RefWidget::isRemove(srcIndex);

    return true;
}

void ModelerIDEPlug::remove()
{
    QModelIndex currentIndex = treeClassView->currentIndex();

    if (currentIndex.isValid()){

        if (!isRemove(currentIndex))
            return;
        closePropWindow(currentIndex);
        m_model->removeRow(currentIndex.row(),currentIndex.parent());
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить узел, поскольку он не выбран."));
}

void ModelerIDEPlug::showPropClass(const QModelIndex &indexSource)
{
    if (!indexSource.isValid())
        return;

    if (indexSource.data(TreeXmlModel::TagRole)!=DBCLASSXML::CLASS)
        return;

    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString subWindowName = "PropClass::" + this->dataId(indexSource);
    QMdiSubWindow* subWindow = mainWindow->setActiveSubWindow(subWindowName);

    if (!subWindow) {
        PropClass* propClass = new PropClass();
        subWindow =  mainWindow->addSubWindow(propClass);
        propClass->setObjectName(subWindowName);
        propClass->setModel(m_model);
        propClass->setCurrentClass(indexSource);
        connect(propClass,SIGNAL(editFilter(QModelIndex)),
                this,SLOT(showPropFilter(QModelIndex)));
    } else {
        PropClass* propClass = qobject_cast<PropClass*>(subWindow->widget());
        propClass->setCurrentClass(indexSource);
    }
}

void ModelerIDEPlug::showPropFilter(const QModelIndex &indexSource)
{

    if (!indexSource.isValid())
        return;

    if (indexSource.data(TreeXmlModel::TagRole)!=DBFILTERXML::FILTER)
        return;

    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString classId = this->dataId(indexSource);

    QString subWindowName = "PropFilter::" + classId;
    QMdiSubWindow* subWindow = mainWindow->setActiveSubWindow(subWindowName);

    if (!subWindow) {
        PropFilter* propFilter = new PropFilter();
        subWindow =  mainWindow->addSubWindow(propFilter);
        propFilter->setObjectName(subWindowName);
        propFilter->setModel(m_model);
        propFilter->setCurrentClass(indexSource);
    } else {
        PropFilter* propFilter = qobject_cast<PropFilter*>(subWindow->widget());
        propFilter->setCurrentClass(indexSource);
    }
}

void ModelerIDEPlug::showPropEntity(const QModelIndex &indexSource)
{
    if (!indexSource.isValid())
        return;

    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString subWindowName = "PropEntity::" + this->dataId(indexSource);
    QMdiSubWindow* subWindow = mainWindow->setActiveSubWindow(subWindowName);

    if (!subWindow) {
        PropEntity* propEntity = new PropEntity();
        subWindow =  mainWindow->addSubWindow(propEntity);
        propEntity->setObjectName(subWindowName);
        propEntity->setModel(m_model);
        propEntity->setCurrentEntity(indexSource);
    } else {
        PropEntity* propEntity = qobject_cast<PropEntity*>(subWindow->widget());
        propEntity->setCurrentEntity(indexSource);
    }
}

void ModelerIDEPlug::showPropEntityGroup(const QModelIndex &indexSource)
{
    if (!indexSource.isValid())
        return;

    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString subWindowName = "PropEntityGroup::" + this->dataId(indexSource);
    QMdiSubWindow* subWindow = mainWindow->setActiveSubWindow(subWindowName);

    if (!subWindow) {
        PropEntityGroup* propEntityGroup = new PropEntityGroup();
        subWindow =  mainWindow->addSubWindow(propEntityGroup);
        propEntityGroup->setObjectName(subWindowName);
        propEntityGroup->setModel(m_model);
        propEntityGroup->setCurrentEntityGroup(indexSource);
    } else {
        PropEntityGroup* propEntityGroup = qobject_cast<PropEntityGroup*>(subWindow->widget());
        propEntityGroup->setCurrentEntityGroup(indexSource);
    }
}

void ModelerIDEPlug::showPropLov(const QModelIndex &indexSource)
{
    if (!indexSource.isValid())
        return;

    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString subWindowName = "PropLov::" + this->dataId(indexSource);
    QMdiSubWindow* subWindow = mainWindow->setActiveSubWindow(subWindowName);

    if (!subWindow) {
        PropLov* propLov = new PropLov();
        subWindow =  mainWindow->addSubWindow(propLov);
        propLov->setObjectName(subWindowName);
        propLov->setModel(m_model);
        propLov->setCurrentLov(indexSource);
    } else {
        PropLov* propLov = qobject_cast<PropLov*>(subWindow->widget());
        propLov->setCurrentLov(indexSource);
    }
}

void ModelerIDEPlug::showPropRefGroup(const QModelIndex &indexSource)
{
    if (!indexSource.isValid())
        return;

    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString subWindowName = "PropRefGroup::" + this->dataId(indexSource);
    QMdiSubWindow* subWindow = mainWindow->setActiveSubWindow(subWindowName);

    if (!subWindow) {
        PropRefGroup* propRefGroup = new PropRefGroup();
        subWindow =  mainWindow->addSubWindow(propRefGroup);
        propRefGroup->setObjectName(subWindowName);
        propRefGroup->setModel(m_model);
        propRefGroup->setCurrentRefGroup(indexSource);
    } else {
        PropRefGroup* propRefGroup = qobject_cast<PropRefGroup*>(subWindow->widget());
        propRefGroup->setCurrentRefGroup(indexSource);
    }
}

void ModelerIDEPlug::showPropRef(const QModelIndex &indexSource)
{
    if (!indexSource.isValid())
        return;

    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString subWindowName = "PropRef::" + this->dataId(indexSource);
    QMdiSubWindow* subWindow = mainWindow->setActiveSubWindow(subWindowName);

    if (!subWindow) {
        PropRef* propRef = new PropRef();
        subWindow =  mainWindow->addSubWindow(propRef);
        propRef->setObjectName(subWindowName);
        propRef->setModel(m_model);
        propRef->setCurrentRef(indexSource);
    } else {
        PropRef* propRef = qobject_cast<PropRef*>(subWindow->widget());
        propRef->setCurrentRef(indexSource);
    }
}

void ModelerIDEPlug::closePropWindow(const QModelIndex &index)
{
    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString subWindowName = this->dataId(index);

    if (index.data(TreeXmlModel::TagRole)==DBCLASSXML::CLASS)
        subWindowName = "PropClass::" + subWindowName;

    if (index.data(TreeXmlModel::TagRole)==DBCOMPXML::COMP)
        subWindowName = "PropComposition::" + subWindowName;

    if (index.data(TreeXmlModel::TagRole)==DBFILTERXML::FILTER)
        subWindowName = "PropFilter::" + subWindowName;

    if (index.data(TreeXmlModel::TagRole)==DBENTITYXML::ENTITY)
        subWindowName = "PropEntiry::" + subWindowName;

    if (index.data(TreeXmlModel::TagRole)==DBENTITYGROUPXML::ENTITYGROUP)
        subWindowName = "PropEntityGroup::" + subWindowName;

    if (index.data(TreeXmlModel::TagRole)==DBLOVXML::LOV)
        subWindowName = "PropLov::" + subWindowName;

    if (index.data(TreeXmlModel::TagRole)==DBREFGROUPXML::REFGROUP)
        subWindowName = "PropRefGroup::" + subWindowName;

    if (index.data(TreeXmlModel::TagRole)==DBREFXML::REF)
        subWindowName = "PropRef::" + subWindowName;


    QMdiSubWindow *subWindow = mainWindow->subWindow(subWindowName);
    if (subWindow)
        subWindow->close();
}

void ModelerIDEPlug::newClassModel()
{
    closeClassModel();
    createClassModel();
}

void ModelerIDEPlug::openClassModel()
{
    m_fileName = QFileDialog::getOpenFileName(NULL, tr("Открытие модели"),
                                                    ".", tr("Фаил модели (*.xml)"));
    if (m_fileName.isEmpty())
        return;

    QFile file(m_fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QDomDocument document;
        if (document.setContent(&file)) {
           createClassModel(document);
        }
        file.close();
    }
    actionSaveModel->setDisabled(true);
}

void ModelerIDEPlug::saveClassModel()
{
    if (m_fileName.isEmpty()) {
        saveAsClassModel();
        return;
    }

    QFile File(m_fileName);
    if ( File.open(QIODevice::WriteOnly) )
    {
        QTextStream TextStream(&File);
        QDomDocument doc = m_model->rootItem()->node().toDocument();
        doc.save(TextStream, 0);
        File.close();
    }
    actionSaveModel->setDisabled(true);
}

void ModelerIDEPlug::saveAsClassModel()
{
    m_fileName = QFileDialog::getSaveFileName(NULL, tr("Сохранение модели"),
                                                    "", tr("Фаил модели (*.xml)"));
    QFile File(m_fileName);
    if ( File.open(QIODevice::WriteOnly) )
    {
        QTextStream TextStream(&File);
        QDomDocument doc = m_model->rootItem()->node().toDocument();
        doc.save(TextStream, 0);
        File.close();
    }
    actionSaveModel->setDisabled(true);
}

void ModelerIDEPlug::publishClassModel(const QModelIndex &index)
{
    int row=0;
    QModelIndex childIndex = m_model->index(row,0,index);
    while (childIndex.isValid())
    {
        if (childIndex.data(TreeXmlModel::TagRole)==DBATTRXML::ATTR){
            // Создание атрибута
            if (m_model->isInherited(childIndex))
                qDebug() << "Унаследованный атрибут:" << childIndex.data().toString();
            else
                qDebug() << "Атрибут:" << childIndex.data().toString();
        } else if (childIndex.data(TreeXmlModel::TagRole)==DBCOMPXML::COMP) {
            // Создание состава
            if (m_model->isInherited(childIndex))
                qDebug() << "Унаследованный состав:" <<childIndex.data().toString();
            else
                qDebug() << "Состав:" <<childIndex.data().toString();
        } else if (childIndex.data(TreeXmlModel::TagRole)==DBCLASSXML::CLASS) {
            // Создание класса
            qDebug() << "Класс:" << childIndex.data().toString();
        }
        publishClassModel(childIndex);
        childIndex = m_model->index(++row,0,index);
    }
}

void ModelerIDEPlug::closeClassModel()
{   
    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));
    foreach (QMdiSubWindow* subWindow, mainWindow->subWindowList())
        if (subWindow->widget()->objectName().indexOf(QRegExp("^PropClass::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropFilter::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropEntityGroup::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropEntity::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropLov::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropRefGroup::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropRef::"))  != -1
        ) {
            subWindow->close();
        }

    if (m_model){
        delete m_model;
        m_model = NULL;
        actionCloseModel->setDisabled(true);
        actionSaveModel->setDisabled(true);
        actionSaveAsModel->setDisabled(true);
        actionPublishModel->setDisabled(true);
    }
}
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(modeleride, ModelerIDEPlug)
#endif
