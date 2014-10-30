#include "classtreeview.h"
#include "dbxmlstruct.h"
#include <treexmlmodel/treexmlmodel.h>

ClassTreeView::ClassTreeView(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    treeView->resize(10,10);
    treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setDropIndicatorShown(true);

    connect(treeView,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(treeDoubleClicked(QModelIndex)));

    // Создание контекстного меню
    contextMenu = new QMenu();

    actionAddClass = new QAction(tr("Добавить"),this);
    contextMenu->addAction(actionAddClass);
    connect(actionAddClass,SIGNAL(triggered()),this,SLOT(add()));

    actionRemoveClass = new QAction(tr("Удалить"),this);
    contextMenu->addAction(actionRemoveClass);
    connect(actionRemoveClass,SIGNAL(triggered()),this,SLOT(remove()));

    actionSeparator = new QAction(tr("Разделитель"),this);
    actionSeparator->setSeparator(true);
    contextMenu->addAction(actionSeparator);

    actionShowAttr = new QAction(tr("Показать атрибуты"),this);
    actionShowAttr->setCheckable(true);
    contextMenu->addAction(actionShowAttr);
    connect(actionShowAttr,SIGNAL(triggered(bool)),this,SLOT(setShowAttr(bool)));

    actionShowComp = new QAction(tr("Показать состав"),this);
    actionShowComp->setCheckable(true);
    contextMenu->addAction(actionShowComp);
    connect(actionShowComp,SIGNAL(triggered(bool)),this,SLOT(setShowComp(bool)));

    actionShowFilter = new QAction(tr("Показать фильтры"),this);
    actionShowFilter->setCheckable(true);
    contextMenu->addAction(actionShowFilter);
    connect(actionShowFilter,SIGNAL(triggered(bool)),this,SLOT(setShowFilter(bool)));

    actionShowUnit = new QAction(tr("Показать ЕИ"),this);
    actionShowUnit->setCheckable(true);
    contextMenu->addAction(actionShowUnit);
    connect(actionShowUnit,SIGNAL(triggered(bool)),this,SLOT(setShowUnit(bool)));

    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView,SIGNAL(customContextMenuRequested(const QPoint&)),
            this,SLOT(showContextMenu(const QPoint&)));

    classFilterModel = NULL;
}

ClassTreeView::~ClassTreeView()
{
    destroyModel();

    delete actionAddClass;
    delete actionRemoveClass;
    delete actionShowAttr;
    delete actionShowComp;
    delete actionShowFilter;
    delete actionSeparator;
    delete actionShowUnit;
    delete contextMenu;
}

void ClassTreeView::setModel(QAbstractItemModel *model)
{
    destroyModel();
    classFilterModel = new TreeFilterProxyModel();
    classFilterModel->setSourceModel(model);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);

    classFilterModel->addVisibleTag(DBCLASSXML::CLASS);
    classFilterModel->addVisibleTag(DBCLASSLISTXML::CLASSLIST);
    classFilterModel->addVisibleTag(DBENTITYGROUPXML::ENTITYGROUP);
    classFilterModel->addVisibleTag(DBENTITYXML::ENTITY);
    classFilterModel->addVisibleTag(DBENTITYLISTXML::ENTITYLIST);
    classFilterModel->addVisibleTag(DBLOVLISTXML::LOVLIST);
    classFilterModel->addVisibleTag(DBLOVXML::LOV);
    classFilterModel->addVisibleTag(DBREFLISTXML::REFLIST);
    classFilterModel->addVisibleTag(DBREFGROUPXML::REFGROUP);
    classFilterModel->addVisibleTag(DBREFXML::REF);
    classFilterModel->addVisibleTag(DBMODELXML::MODEL);
    classFilterModel->addVisibleTag(DBROOTXML::ROOT);

    QRegExp regex = classFilterModel->filterRegExp();
    classFilterModel->setFilterRegExp(regex);

    connect(model, SIGNAL(destroyed()), this, SLOT(destroyModel()));

    connect(lineEditFiler,SIGNAL(textChanged(QString)),
            classFilterModel,SLOT(setFilterRegExp(QString)));

    connect(lineEditFiler,SIGNAL(textChanged(QString)),
            treeView,SLOT(expandAll()));

    treeView->setModel(classFilterModel);

    for (int i=1;i<model->columnCount();i++)
        treeView->hideColumn(i);

    actionShowAttr->setChecked(false);
    actionShowComp->setChecked(false);
    actionShowFilter->setChecked(false);
    actionShowUnit->setChecked(false);
}

QModelIndex ClassTreeView::currentIndex() const
{
    return classFilterModel->mapToSource(treeView->currentIndex());
}

void ClassTreeView::setShowAttr(bool shown)
{
    if (shown)
        classFilterModel->addVisibleTag(DBATTRXML::ATTR);
    else
        classFilterModel->removeVisibleTag(DBATTRXML::ATTR);

    QRegExp regex = classFilterModel->filterRegExp();
    classFilterModel->setFilterRegExp(regex);
    actionShowAttr->setChecked(shown);
}

void ClassTreeView::setShowComp(bool shown)
{
    if (shown)
        classFilterModel->addVisibleTag(DBCOMPXML::COMP);
    else
        classFilterModel->removeVisibleTag(DBCOMPXML::COMP);

    QRegExp regex = classFilterModel->filterRegExp();
    classFilterModel->setFilterRegExp(regex);
    actionShowComp->setChecked(shown);
}

void ClassTreeView::setShowFilter(bool shown)
{
    if (shown)
        classFilterModel->addVisibleTag(DBFILTERXML::FILTER);
    else
        classFilterModel->removeVisibleTag(DBFILTERXML::FILTER);

    QRegExp regex = classFilterModel->filterRegExp();
    classFilterModel->setFilterRegExp(regex);
    actionShowFilter->setChecked(shown);
}

void ClassTreeView::setShowUnit(bool shown)
{
    if (shown)
        classFilterModel->addVisibleTag(DBUNITXML::UNIT);
    else
        classFilterModel->removeVisibleTag(DBUNITXML::UNIT);

    QRegExp regex = classFilterModel->filterRegExp();
    classFilterModel->setFilterRegExp(regex);
    actionShowUnit->setChecked(shown);
}

void ClassTreeView::treeDoubleClicked(const QModelIndex &index)
{
    QModelIndex srcIndex = classFilterModel->mapToSource(index);
    emit doubleClicked(srcIndex);
}

void ClassTreeView::destroyModel()
{
    if (classFilterModel){
        delete classFilterModel;
        classFilterModel = NULL;
    }
}

void ClassTreeView::setCurrentIndex(const QModelIndex &index)
{
    treeView->setCurrentIndex(classFilterModel->mapFromSource(index));
}

void ClassTreeView::showContextMenu(const QPoint &point)
{
    if (classFilterModel->sourceModel()){
        QModelIndex indexSource = currentIndex();
        if (!indexSource.isValid())
            return;

        if (indexSource.data(TreeXmlModel::TagRole)==DBCLASSLISTXML::CLASSLIST
            || indexSource.data(TreeXmlModel::TagRole)==DBENTITYLISTXML::ENTITYLIST
            || indexSource.data(TreeXmlModel::TagRole)==DBENTITYGROUPXML::ENTITYGROUP
            || indexSource.data(TreeXmlModel::TagRole)==DBLOVLISTXML::LOVLIST
            || indexSource.data(TreeXmlModel::TagRole)==DBCLASSXML::CLASS
            || indexSource.data(TreeXmlModel::TagRole)==DBREFLISTXML::REFLIST
            || indexSource.data(TreeXmlModel::TagRole)==DBREFGROUPXML::REFGROUP)
        {
            contextMenu->actions().at(0)->setVisible(true);
        } else {
            contextMenu->actions().at(0)->setVisible(false);
        }

        if (indexSource.data(TreeXmlModel::TagRole)==DBCLASSLISTXML::CLASSLIST
            || indexSource.data(TreeXmlModel::TagRole)==DBENTITYLISTXML::ENTITYLIST
            || indexSource.data(TreeXmlModel::TagRole)==DBLOVLISTXML::LOVLIST
            || indexSource.data(TreeXmlModel::TagRole)==DBREFLISTXML::REFLIST
            || indexSource.data(TreeXmlModel::TagRole)==DBMODELXML::MODEL)
        {
            contextMenu->actions().at(1)->setVisible(false);
        } else {
            contextMenu->actions().at(1)->setVisible(true);
        }
        contextMenu->exec(treeView->mapToGlobal(point));
    }
    return;
}
