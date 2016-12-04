#include "classtreeview.h"
#include <metadatamodel/dbxmlstruct.h>
#include <treexmlmodel/treexmlmodel.h>

using namespace RTPTechGroup::XmlModel;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

ClassTreeView::ClassTreeView(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    treeView->resize(10,10);
    treeView->setDragEnabled(true);
    treeView->setAcceptDrops(true);
    treeView->setDropIndicatorShown(true);

    connect(treeView, &QTreeView::doubleClicked,
            this, &ClassTreeView::treeDoubleClicked);

    // Создание контекстного меню
    contextMenu = new QMenu();

    actionAddClass = new QAction(tr("Добавить"),this);
    contextMenu->addAction(actionAddClass);
    connect(actionAddClass, &QAction::triggered, this, &ClassTreeView::actionInsert);

    actionRemoveClass = new QAction(tr("Удалить"),this);
    contextMenu->addAction(actionRemoveClass);
    connect(actionRemoveClass, &QAction::triggered, this, &ClassTreeView::actionRemove);

    actionSeparator = new QAction(tr("Разделитель"),this);
    actionSeparator->setSeparator(true);
    contextMenu->addAction(actionSeparator);

    actionShowAttr = new QAction(tr("Показать атрибуты"),this);
    actionShowAttr->setCheckable(true);
    contextMenu->addAction(actionShowAttr);
    connect(actionShowAttr,&QAction::triggered, this, &ClassTreeView::setShowAttr);

    actionShowComp = new QAction(tr("Показать состав"),this);
    actionShowComp->setCheckable(true);
    contextMenu->addAction(actionShowComp);
    connect(actionShowComp, &QAction::triggered, this, &ClassTreeView::setShowComp);

    actionShowFilter = new QAction(tr("Показать фильтры"),this);
    actionShowFilter->setCheckable(true);
    contextMenu->addAction(actionShowFilter);
    connect(actionShowFilter, &QAction::triggered, this, &ClassTreeView::setShowFilter);

    actionShowPermission = new QAction(tr("Показать права"),this);
    actionShowPermission->setCheckable(true);
    contextMenu->addAction(actionShowPermission);
    connect(actionShowPermission, &QAction::triggered, this, &ClassTreeView::setShowPermission);


    actionShowUnit = new QAction(tr("Показать ЕИ"),this);
    actionShowUnit->setCheckable(true);
    contextMenu->addAction(actionShowUnit);
    connect(actionShowUnit, &QAction::triggered, this, &ClassTreeView::setShowUnit);


    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView, &QTreeView::customContextMenuRequested,
            this, &ClassTreeView::showContextMenu);

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
    classFilterModel->addVisibleTag(DBQUANTITYGROUPXML::QUANTITYGROUP);
    classFilterModel->addVisibleTag(DBQUANTITYXML::QUANTITY);
    classFilterModel->addVisibleTag(DBQUANTITYLISTXML::QUANTITYLIST);
    classFilterModel->addVisibleTag(DBLOVLISTXML::LOVLIST);
    classFilterModel->addVisibleTag(DBLOVXML::LOV);
    classFilterModel->addVisibleTag(DBNUMERATORLISTXML::NUMERATORLIST);
    classFilterModel->addVisibleTag(DBNUMERATORXML::NUMERATOR);
    classFilterModel->addVisibleTag(DBREFLISTXML::REFLIST);
    classFilterModel->addVisibleTag(DBREFGROUPXML::REFGROUP);
    classFilterModel->addVisibleTag(DBREFXML::REF);
    classFilterModel->addVisibleTag(DBROLELISTXML::ROLELIST);
    classFilterModel->addVisibleTag(DBROLEXML::ROLE);
    classFilterModel->addVisibleTag(DBMODELXML::MODEL);
    classFilterModel->addVisibleTag(DBROOTXML::ROOT);

    QRegExp regex = classFilterModel->filterRegExp();
    classFilterModel->setFilterRegExp(regex);

    connect(model, &QAbstractItemModel::destroyed,
            this, &ClassTreeView::destroyModel);

    connect(lineEditFiler, &QLineEdit::textChanged,
            classFilterModel,
            static_cast<void (TreeFilterProxyModel::*)(const QString&)>
            (&TreeFilterProxyModel::setFilterRegExp));

    connect(lineEditFiler, &QLineEdit::textChanged,
            treeView, &QTreeView::expandAll);

    treeView->setModel(classFilterModel);

    for (qint32 i = model->columnCount() - 1; i > 0; --i)
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

void ClassTreeView::setShowPermission(bool shown)
{
    if (shown)
        classFilterModel->addVisibleTag(DBPERMISSIONXML::PERMISSION);
    else
        classFilterModel->removeVisibleTag(DBPERMISSIONXML::PERMISSION);

    QRegExp regex = classFilterModel->filterRegExp();
    classFilterModel->setFilterRegExp(regex);
    actionShowPermission->setChecked(shown);
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

void ClassTreeView::actionInsert()
{
    emit actionInserted();
}

void ClassTreeView::actionRemove()
{
    emit actionRemoved();
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

        QString tagRole = indexSource.data(TreeXmlModel::TagRole).toString();

        //! Команда добавить
        if (tagRole == DBCLASSLISTXML::CLASSLIST
            || tagRole == DBQUANTITYLISTXML::QUANTITYLIST
            || tagRole == DBQUANTITYGROUPXML::QUANTITYGROUP
            || tagRole == DBLOVLISTXML::LOVLIST
            || tagRole == DBNUMERATORLISTXML::NUMERATORLIST
            || tagRole == DBCLASSXML::CLASS
            || tagRole == DBREFLISTXML::REFLIST
            || tagRole == DBREFGROUPXML::REFGROUP
            || tagRole == DBROLELISTXML::ROLELIST)
        {
            contextMenu->actions().at(0)->setVisible(true);
        } else {
            contextMenu->actions().at(0)->setVisible(false);
        }

        //! Команда удалить
        if (tagRole == DBCLASSLISTXML::CLASSLIST
            || tagRole == DBQUANTITYLISTXML::QUANTITYLIST
            || tagRole == DBLOVLISTXML::LOVLIST
            || tagRole == DBNUMERATORLISTXML::NUMERATORLIST
            || tagRole == DBREFLISTXML::REFLIST
            || tagRole == DBROLELISTXML::ROLELIST
            || tagRole == DBMODELXML::MODEL)
        {
            contextMenu->actions().at(1)->setVisible(false);
        } else {
            contextMenu->actions().at(1)->setVisible(true);
        }

        QModelIndex indexParentSource = indexSource.parent();
        if (!indexParentSource.isValid())
            return;

        if (tagRole == DBATTRXML::ATTR
            && indexParentSource.data(TreeXmlModel::TagRole)==DBCLASSLISTXML::CLASSLIST)
            contextMenu->actions().at(1)->setVisible(false);

        contextMenu->exec(treeView->mapToGlobal(point));
    }
    return;
}

}}
