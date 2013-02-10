#include <QMainWindow>
#include <QFileDialog>
#include <QMenu>
#include <QToolBar>

#include <itreedockwidget.h>
#include <imainwindow.h>

#include <mdiextarea/mdiextarea.h>
#include <treexmlmodel/tagxmlitem.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <plugin/pluginmanager.h>

#include "modelerideplug.h"
#include "propclass.h"
#include "propcomposition.h"
#include "dbxmlstruct.h"

ModelerIDEPlug::ModelerIDEPlug(QObject *parent):
    QObject(parent), IPlugin("ITreeDockWidget IMainWindow")
{
    m_model = NULL;

    PluginManager* pluginManager = PluginManager::instance();
    ITreeDockWidget* dockWidget = qobject_cast<ITreeDockWidget*>(
                pluginManager->interfaceObject("ITreeDockWidget"));
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
    QMainWindow* mainWindow = qobject_cast<QMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    IMainWindow* iMainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    // QMenu* menuFile = iMainWindow->getMenuFile();

    QAction* separator;

    actionNewModel = new QAction(QIcon(":newmodel"), tr("Новая модель"), mainWindow);
    connect(actionNewModel, SIGNAL(triggered()), this, SLOT(newClassModel()));
    actionNewModel->setObjectName("actionNewModel");
    iMainWindow->addAction(tr("Редактор модели"),actionNewModel);

    actionOpenModel = new QAction(QIcon(":openmodel"), tr("Открыть модель..."), mainWindow);
    connect(actionOpenModel, SIGNAL(triggered()), this, SLOT(openClassModel()));
    actionOpenModel->setObjectName("actionOpenModel");
    iMainWindow->addAction(tr("Редактор модели"),actionOpenModel);

    actionSaveModel = new QAction(QIcon(":savemodel"), tr("Сохранить модель"), mainWindow);
    connect(actionSaveModel, SIGNAL(triggered()), this, SLOT(saveClassModel()));
    actionSaveModel->setDisabled(true);
    actionSaveModel->setObjectName("actionSaveModel");
    iMainWindow->addAction(tr("Редактор модели"),actionSaveModel);

    actionSaveAsModel = new QAction(QIcon(":savemodel"), tr("Сохранить модель как..."), mainWindow);
    connect(actionSaveAsModel, SIGNAL(triggered()), this, SLOT(saveAsClassModel()));
    actionSaveAsModel->setDisabled(true);
    actionSaveAsModel->setObjectName("actionSaveAsModel");
    iMainWindow->addAction(tr("Редактор модели"),actionSaveAsModel);

    actionPublishModel = new QAction(QIcon(":publish"), tr("Опубликовать модель..."), mainWindow);
    connect(actionPublishModel, SIGNAL(triggered()), this, SLOT(publishClassModel()));
    actionPublishModel->setDisabled(true);
    actionPublishModel->setObjectName("actionPublishModel");
    iMainWindow->addAction(tr("Редактор модели"),actionPublishModel);

    actionCloseModel = new QAction(QIcon(":closemodel"), tr("Закрыть модель"), mainWindow);
    connect(actionCloseModel, SIGNAL(triggered()), this, SLOT(closeClassModel()));
    actionCloseModel->setDisabled(true);
    actionCloseModel->setObjectName("actionCloseModel");
    iMainWindow->addAction(tr("Редактор модели"),actionCloseModel);

    QToolBar* toolBarMain = iMainWindow->getToolBarMain();

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
}

ModelerIDEPlug::~ModelerIDEPlug()
{
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

void ModelerIDEPlug::showContextMenu(const QPoint& point)
{
    if (m_model){
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

void ModelerIDEPlug::actionSaveEnable()
{
    actionSaveModel->setEnabled(true);
}

TreeXMLModel *ModelerIDEPlug::model()
{
    return m_model;
}

void ModelerIDEPlug::createClassModel(QDomDocument document)
{
    m_model = new TreeXMLModel(document, this);

    m_model->addTagFilter(DBCLASSXML::CLASS);
    m_model->addTagFilter(DBATTRXML::ATTR);
    m_model->addTagFilter(DBCOMPXML::COMP);

    QStringList propsClass;
    propsClass << DBCLASSXML::NAME     << DBCLASSXML::ISABSTARCT  <<
                  DBCLASSXML::TYPE     << DBCLASSXML::DESCRIPTION <<
                  DBCLASSXML::PARENT   << DBCLASSXML::ISACTIVE    <<
                  DBCLASSXML::TEMPLATE << DBCLASSXML::ID          <<
                  DBCLASSXML::ICON;
    m_model->addDisplayedAttr(DBCLASSXML::CLASS,propsClass,QIcon(":/modeleride"));
    m_model->addDisplayedAttr(DBATTRXML::ATTR,propsClass,QIcon(":/attribute"));
    m_model->addDisplayedAttr(DBCOMPXML::COMP,propsClass,QIcon(":/composition"));

    QStringList propsAttr;
    propsAttr << DBATTRXML::NAME           << DBATTRXML::DESCRIPTION
              << DBATTRXML::TYPE           << DBATTRXML::MAXSTRLEN
              << DBATTRXML::REFCLASS       << DBATTRXML::PARENT
              << DBATTRXML::INITIALVAL     << DBATTRXML::GROUP
              << DBATTRXML::ISNULLALLOWED  << DBATTRXML::ISUNIQUE
              << DBATTRXML::ISCANDIDATEKEY << DBATTRXML::ID;

    QStringList propsComposition;
    propsComposition << DBCOMPXML::NAME              << DBCOMPXML::DESCRIPTION
                     << DBCOMPXML::PARENT            << DBCOMPXML::CLASS
                     << DBCOMPXML::DIRECTDESCRIPTION << DBCOMPXML::INVERSEDESCRIPTION
                     << DBCOMPXML::ID;

    m_model->setHeaderData(0, Qt::Horizontal, tr("Имя атрибута"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("Описание"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("Тип"));
    m_model->setHeaderData(3, Qt::Horizontal, tr("Длина строки"));
    m_model->setHeaderData(4, Qt::Horizontal, tr("Ссылочный класс"));
    m_model->setHeaderData(5, Qt::Horizontal, tr("Класс"));
    m_model->setHeaderData(6, Qt::Horizontal, tr("По умолчанию"));
    m_model->setHeaderData(7, Qt::Horizontal, tr("Группа"));
    m_model->setHeaderData(8, Qt::Horizontal, tr("Нулевые значения"));
    m_model->setHeaderData(9, Qt::Horizontal, tr("Уникальный"));
    m_model->setHeaderData(10, Qt::Horizontal, tr("Кандидат в ключ"));

    m_model->addDisplayedAttr(DBATTRXML::ATTR,propsAttr);
    m_model->addAttrTag(DBATTRXML::ATTR);

    m_model->addDisplayedAttr(DBCOMPXML::COMP,propsComposition);
    m_model->addAttrTag(DBCOMPXML::COMP);

    QStringList insertTags;
    insertTags << DBATTRXML::ATTR << DBCLASSXML::CLASS << DBCOMPXML::COMP;
    m_model->addInsertTags(DBCLASSXML::CLASS,insertTags);
    insertTags.clear();
    insertTags << DBATTRXML::ATTR;
    m_model->addInsertTags(DBCOMPXML::COMP,insertTags);

    m_model->addHashAttr(DBCLASSXML::CLASS,
                                DBCLASSXML::NAME,
                                TreeXMLModel::UniqueRename);
    m_model->addHashAttr(DBATTRXML::ATTR,
                                DBATTRXML::REFCLASS,
                                TreeXMLModel::NoUnique);
    m_model->addHashAttr(DBCOMPXML::COMP,
                                DBCOMPXML::CLASS,
                                TreeXMLModel::NoUnique);

    m_model->addHashAttr(DBCLASSXML::CLASS,
                                DBCLASSXML::ID,
                                TreeXMLModel::Uuid);
    m_model->addHashAttr(DBATTRXML::ATTR,
                                DBATTRXML::ID,
                                TreeXMLModel::Uuid);
    m_model->addHashAttr(DBCOMPXML::COMP,
                                DBCOMPXML::ID,
                                TreeXMLModel::Uuid);

    m_model->addRelation(DBATTRXML::ATTR,DBATTRXML::REFCLASS,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);
    m_model->addRelation(DBATTRXML::ATTR,DBATTRXML::PARENT,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);
    m_model->addRelation(DBATTRXML::ATTR,DBATTRXML::PARENT,
                               DBCOMPXML::COMP, DBCOMPXML::NAME);

    m_model->addRelation(DBCLASSXML::CLASS, DBCLASSXML::PARENT,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);

    m_model->addRelation(DBCOMPXML::COMP, DBCOMPXML::PARENT,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);
    m_model->addRelation(DBCOMPXML::COMP, DBCOMPXML::CLASS,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);

    m_model->refreshHashing();

    classFilterModel = new TreeFilterProxyModel();
    classFilterModel->setSourceModel(m_model);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);

    setShownComp(false);   
    setShownAttr(false);

    connect(m_model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this,SLOT(actionSaveEnable()));
    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(actionSaveEnable()));

    connect(treeClassView->lineEditFiler,SIGNAL(textChanged(QString)),
            classFilterModel,SLOT(setFilterRegExp(QString)));

    connect(treeClassView->lineEditFiler,SIGNAL(textChanged(QString)),
            treeClassView->treeView,SLOT(expandAll()));

    treeClassView->treeView->setModel(classFilterModel);
    for (int i=1;i<m_model->columnCount();i++)
        treeClassView->treeView->hideColumn(i);

    actionPublishModel->setEnabled(true);
    actionSaveModel->setEnabled(true);
    actionSaveAsModel->setEnabled(true);
    actionCloseModel->setEnabled(true);
}

void ModelerIDEPlug::addClass()
{
    m_model->setInsTagName(DBCLASSXML::CLASS);
    QModelIndex indexSource = classFilterModel->mapToSource(treeClassView->treeView->currentIndex());
    if (m_model->insertRow(0,indexSource)){
        QModelIndex index = classFilterModel->mapFromSource(m_model->lastInsertRow());
        treeClassView->treeView->setCurrentIndex(index);
        showPropClass(m_model->lastInsertRow());
    }
}

QString ModelerIDEPlug::className(const QModelIndex& index)
{
    return index.sibling(index.row(),m_model->columnDisplayedAttr(
                             DBCLASSXML::CLASS,
                             DBCLASSXML::NAME
                             )).data().toString();
}

QString ModelerIDEPlug::classId(const QModelIndex& index)
{
    return index.sibling(index.row(),m_model->columnDisplayedAttr(
                             DBCLASSXML::CLASS,
                             DBCLASSXML::ID
                             )).data().toString();
}

void ModelerIDEPlug::dblClickTree(const QModelIndex &index)
{
    QModelIndex indexSource = classFilterModel->mapToSource(index);

    if (!indexSource.isValid())
        return;

    if (!m_model->isAttr(indexSource))
        showPropClass(indexSource);

    if (indexSource.data(Qt::UserRole)==DBCOMPXML::COMP)
        showPropComposition(indexSource);
}

// Метод совпадает с bool ClassWidget::isRemove(const QModelIndex &srcIndex)
bool ModelerIDEPlug::isRemoveClass(const QModelIndex &srcIndex)
{
    bool success = true;
    QString msg;

    QString tag = srcIndex.data(Qt::UserRole).toString();
    QStringList tags;
    tags << tag;
    if (m_model->rowCount(srcIndex,tags)) {
        msg += tr("Необходимо удалить классы-потомки.\n\n");
        if (success)
            success = false;
    }

    QString fieldId = m_model->uuidAttr(tag);
    if (fieldId.isEmpty())
        return true;

    QString guid =  srcIndex.sibling(srcIndex.row(),
                                     m_model->columnDisplayedAttr(
                                         tag,fieldId))
            .data().toString();

    foreach (TreeXMLModel::TagWithAttr tagWithAttr,
             m_model->fromRelation(tag))
    {
        int number = 0;

        QModelIndex linkIndex = m_model->indexHashAttr(
                    tagWithAttr.tag,
                    tagWithAttr.attr,
                    guid,
                    number
                    );

        while (linkIndex.isValid()) {
            QModelIndex linkParent = linkIndex.parent();
            if (linkParent.sibling(linkIndex.parent().row(),0)!= srcIndex){
                QString parentName;
                QString name;
                if (linkIndex.data(Qt::UserRole) == DBCLASSXML::CLASS) {
                    name = tr("класс ")
                            + linkIndex.sibling(linkIndex.row(),
                                                m_model->columnDisplayedAttr(
                                                    DBCLASSXML::CLASS,
                                                    DBCLASSXML::NAME)
                                                ).data().toString();
                } else {
                    if (linkParent.data(Qt::UserRole) == DBCOMPXML::COMP)
                        parentName = tr(" принадлежащий составу ")
                                + linkParent.sibling(
                                    linkParent.row(),
                                    m_model->columnDisplayedAttr(
                                        DBCOMPXML::COMP,
                                        DBCOMPXML::NAME)
                                    ).data().toString();
                    else
                        parentName = tr(" принадлежащий классу ")
                                + linkParent.sibling(
                                    linkParent.row(),
                                    m_model->columnDisplayedAttr(
                                        DBCLASSXML::CLASS,
                                        DBCLASSXML::NAME)
                                    ).data().toString();

                    if  (linkIndex.data(Qt::UserRole) == DBCOMPXML::COMP)
                        name = tr("состав ")
                                + linkIndex.sibling(linkIndex.row(),
                                                    m_model->columnDisplayedAttr(
                                                        DBCOMPXML::COMP,
                                                        DBCOMPXML::NAME)
                                                    ).data().toString();
                    else
                        name = tr("атрибут ")
                                + linkIndex.sibling(linkIndex.row(),
                                                    m_model->columnDisplayedAttr(
                                                        DBATTRXML::ATTR,
                                                        DBATTRXML::NAME)
                                                    ).data().toString();

                }
                msg += QString(tr("Необходимо удалить %1%2.\n\n")).
                        arg(name).arg(parentName);
                if (success)
                    success = false;
            }
            number++;
            linkIndex = m_model->indexHashAttr(
                        tagWithAttr.tag,
                        tagWithAttr.attr,
                        guid,
                        number
                        );
        }
    }
    if (!success) {
        QMessageBox msgBox;
        msgBox.setText(tr("Удаление данного объекта не воможно."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setDetailedText(msg);
        msgBox.setWindowTitle(tr("Предупреждение"));
        msgBox.exec();
    }
    return success;
}

void ModelerIDEPlug::removeClass()
{
    QModelIndex currentIndex = classFilterModel->mapToSource(
                treeClassView->treeView->currentIndex());

    if (currentIndex.isValid()){
        if (!className(currentIndex).isEmpty())
        {
            if (!isRemoveClass(currentIndex))
                return;

            PluginManager* pluginManager = PluginManager::instance();
            IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                        pluginManager->interfaceObject("IMainWindow"));

            QString subWindowName = "PropClass::" + this->classId(currentIndex);
            QMdiSubWindow *subWindow = mainWindow->subWindow(subWindowName);
            if (subWindow)
                subWindow->close();
            m_model->removeRow(currentIndex.row(),currentIndex.parent());
        } else
            QMessageBox::warning(NULL,tr("Предупреждение"),
                                 tr("Невозможно удалить узел, поскольку он еще не сохранен."));
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить узел, поскольку он не выбран."));
}

void ModelerIDEPlug::showPropClass(const QModelIndex &indexSource)
{
    if (!indexSource.isValid())
        return;

    if (m_model->isAttr(indexSource))
        return;

    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString subWindowName = "PropClass::" + this->classId(indexSource);
    QMdiSubWindow* subWindow = mainWindow->setActiveSubWindow(subWindowName);

    if (!subWindow) {
        PropClass* propClass = new PropClass();
        subWindow =  mainWindow->addSubWindow(propClass);
        propClass->setObjectName(subWindowName);
        propClass->setModel(m_model);
        propClass->setCurrentClass(indexSource);
        connect(propClass,SIGNAL(editComposition(QModelIndex)),
                this,SLOT(showPropComposition(QModelIndex)));
    } else {
        PropClass* propClass = qobject_cast<PropClass*>(subWindow->widget());
        propClass->setCurrentClass(indexSource);
    }
}

void ModelerIDEPlug::showPropComposition(const QModelIndex &indexSource)
{
    if (!indexSource.isValid())
        return;

    if (indexSource.data(Qt::UserRole)!=DBCOMPXML::COMP)
        return;

    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString className = indexSource.sibling(indexSource.row(),m_model->columnDisplayedAttr(
                                 DBCOMPXML::COMP,
                                 DBCOMPXML::NAME
                                 )).data().toString();

    QString classId = indexSource.sibling(indexSource.row(),m_model->columnDisplayedAttr(
                                 DBCOMPXML::COMP,
                                 DBCOMPXML::NAME
                                 )).data().toString();

    QString subWindowName = "PropComposition::" + classId;
    QMdiSubWindow* subWindow = mainWindow->setActiveSubWindow(subWindowName);

    if (!subWindow) {
        PropComposition* propComposition = new PropComposition();
        subWindow =  mainWindow->addSubWindow(propComposition);
        propComposition->setObjectName(subWindowName);
        propComposition->setModel(m_model);
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
    for (int row=0;row < m_model->rowCount(index);row++){
        QModelIndex childIndex = m_model->index(row,0,index);
        if (childIndex.data(Qt::UserRole)==DBATTRXML::ATTR){
            // Создание атрибута
            if (m_model->isInherited(childIndex))
                qDebug() << "Унаследованный атрибут:" << childIndex.data().toString();
            else
                qDebug() << "Атрибут:" << childIndex.data().toString();
        } else if (childIndex.data(Qt::UserRole)==DBCOMPXML::COMP) {
            // Создание состава
            if (m_model->isInherited(childIndex))
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
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));
    foreach (QMdiSubWindow* subWindow, mainWindow->subWindowList())
        if (subWindow->widget()->objectName().indexOf(QRegExp("^PropClass::"))
                || subWindow->widget()->objectName().indexOf(QRegExp("^PropComposition::")))
            subWindow->close();

    if (m_model){
        delete classFilterModel;
        classFilterModel = NULL;
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
