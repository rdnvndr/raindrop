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
#include "propcomposition.h"
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
    ITreeDockWidget* dockWidget = qobject_cast<ITreeDockWidget*>(
                pluginManager->interfaceObject("ITreeDockWidget"));

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
    m_model = new TreeXmlHashModel(document, this);

    m_model->addTagFilter(DBCLASSXML::CLASS);
    m_model->addTagFilter(DBATTRXML::ATTR);
    m_model->addTagFilter(DBCOMPXML::COMP);
    m_model->addTagFilter(DBFILTERXML::FILTER);
    m_model->addTagFilter(DBFILTERBLOCKXML::BLOCK);
    m_model->addTagFilter(DBCONDITIONXML::COND);
    m_model->addTagFilter(DBENTITYXML::ENTITY);
    m_model->addTagFilter(DBUNITXML::UNIT);
    m_model->addTagFilter(DBENTITYGROUPXML::ENTITYGROUP);
    m_model->addTagFilter(DBCLASSLISTXML::CLASSLIST);
    m_model->addTagFilter(DBENTITYLISTXML::ENTITYLIST);
    m_model->addTagFilter(DBMODELXML::MODEL);
    m_model->addTagFilter(DBLOVLISTXML::LOVLIST);
    m_model->addTagFilter(DBLOVXML::LOV);
    m_model->addTagFilter(DBLOVVALUEXML::LOVVALUE);
    m_model->addTagFilter(DBREFLISTXML::REFLIST);
    m_model->addTagFilter(DBREFGROUPXML::REFGROUP);
    m_model->addTagFilter(DBREFXML::REF);
    m_model->addTagFilter(DBLINKTOCLASSXML::LINKTOCLASS);
    m_model->addTagFilter(DBLINKTOFILTERXML::LINKTOFILTER);

    QStringList propsClass;
    propsClass << DBCLASSXML::NAME     << DBCLASSXML::ISABSTARCT  <<
                  DBCLASSXML::TYPE     << DBCLASSXML::ALIAS <<
                  DBCLASSXML::PARENT   << DBCLASSXML::ISACTIVE    <<
                  DBCLASSXML::TEMPLATE << DBCLASSXML::ID          <<
                  DBCLASSXML::ICON;
    m_model->addDisplayedAttr(DBCLASSXML::CLASS,propsClass,QIcon(":/modeleride"));

    QStringList propsAttr;
    propsAttr << DBATTRXML::NAME           << DBATTRXML::ALIAS
              << DBATTRXML::TYPE           << DBATTRXML::MAXSTRLEN
              << DBATTRXML::REFCLASS       << DBATTRXML::PARENT
              << DBATTRXML::REFUNIT        << DBATTRXML::INITIALVAL
              << DBATTRXML::LOWERBOUND     << DBATTRXML::UPPERBOUND
              << DBATTRXML::REFLOV         << DBATTRXML::GROUP
              << DBATTRXML::ISNULLALLOWED  << DBATTRXML::ISUNIQUE
              << DBATTRXML::ISCANDIDATEKEY << DBATTRXML::ID;
    m_model->addDisplayedAttr(DBATTRXML::ATTR,propsAttr, QIcon(":/attribute"));
    m_model->addAttrTag(DBATTRXML::ATTR);

    QStringList propsComposition;
    propsComposition << DBCOMPXML::NAME              << DBCOMPXML::ALIAS
                     << DBCOMPXML::PARENT            << DBCOMPXML::CLASS
                     << DBCOMPXML::ISVIEW            << DBCOMPXML::ISCOMP
                     << DBCOMPXML::DIRECTDESCRIPTION << DBCOMPXML::INVERSEDESCRIPTION
                     << DBCOMPXML::ID;
    m_model->addDisplayedAttr(DBCOMPXML::COMP,propsComposition, QIcon(":/composition"));
    m_model->addAttrTag(DBCOMPXML::COMP);

    QStringList propsFilter;
    propsFilter << DBFILTERXML::NAME              << DBFILTERXML::ALIAS
                << DBFILTERXML::PARENT            << DBFILTERXML::CLASS
                << DBFILTERXML::DIRECTDESCRIPTION << DBFILTERXML::INVERSEDESCRIPTION
                << DBFILTERXML::ID;
    m_model->addDisplayedAttr(DBFILTERXML::FILTER,propsFilter, QIcon(":/filter"));
    m_model->addAttrTag(DBFILTERXML::FILTER);

    QStringList propsFilterBlock;
    propsFilterBlock << DBFILTERBLOCKXML::LINKOF << DBFILTERBLOCKXML::PARENT
                     << DBFILTERBLOCKXML::ID;
    m_model->addDisplayedAttr(DBFILTERBLOCKXML::BLOCK, propsFilterBlock,
                              QIcon(":/block"));
    m_model->addAttrTag(DBFILTERBLOCKXML::BLOCK);

    QStringList propsCondition;
    propsCondition   << DBCONDITIONXML::FIRSTATTR   << DBCONDITIONXML::OPERATOR
                     << DBCONDITIONXML::SECONDATTR  << DBCONDITIONXML::LINKOF
                     << DBCONDITIONXML::PARENT      << DBCONDITIONXML::ID;
    m_model->addDisplayedAttr(DBCONDITIONXML::COND, propsCondition,
                              QIcon(":/expression"));
    m_model->addAttrTag(DBCONDITIONXML::COND);

    QStringList propsEntity;
    propsEntity << DBENTITYXML::NAME      << DBENTITYXML::DESCRIPTION
                << DBENTITYXML::DIMENSIONSYMBOL
                << DBENTITYXML::BASICUNIT << DBENTITYXML::ID;
    m_model->addDisplayedAttr(DBENTITYXML::ENTITY,propsEntity, QIcon(":/entity"));

    QStringList propsUnit;
    propsUnit << DBUNITXML::NAME           << DBUNITXML::CODE
              << DBUNITXML::COEFF          << DBUNITXML::DELTA
              << DBUNITXML::DESIGNATION    << DBUNITXML::SYMBOL
              << DBUNITXML::INTDESIGNATION << DBUNITXML::INTSYMBOL
              << DBUNITXML::PARENT         << DBUNITXML::ID;
    m_model->addDisplayedAttr(DBUNITXML::UNIT,propsUnit, QIcon(":/unit"));

    QStringList propsGroup;
    propsGroup << DBENTITYGROUPXML::NAME   << DBENTITYGROUPXML::DESCRIPTION
                   << DBENTITYGROUPXML::PARENT << DBENTITYGROUPXML::ID;
    m_model->addDisplayedAttr(DBENTITYGROUPXML::ENTITYGROUP,propsGroup, QIcon(":/entitygroup"));

    QStringList propsClassList;
    propsClassList << DBCLASSLISTXML::NAME   << DBCLASSLISTXML::ALIAS
                   << DBCLASSLISTXML::PARENT << DBCLASSLISTXML::ID;
    m_model->addDisplayedAttr(DBCLASSLISTXML::CLASSLIST,propsClassList, QIcon(":/classes"));

    QStringList propsEntityList;
    propsEntityList << DBENTITYLISTXML::NAME   << DBENTITYLISTXML::ALIAS
                    << DBENTITYLISTXML::PARENT << DBENTITYLISTXML::ID;
    m_model->addDisplayedAttr(DBENTITYLISTXML::ENTITYLIST, propsEntityList, QIcon(":/units"));

    QStringList propsLovList;
    propsLovList << DBLOVLISTXML::NAME   << DBLOVLISTXML::ALIAS
                 << DBLOVLISTXML::PARENT << DBLOVLISTXML::ID;
    m_model->addDisplayedAttr(DBLOVLISTXML::LOVLIST, propsLovList, QIcon(":/lovlist"));

    QStringList propsRefList;
    propsRefList << DBREFLISTXML::NAME   << DBREFLISTXML::ALIAS
                 << DBREFLISTXML::PARENT << DBREFLISTXML::ID;
    m_model->addDisplayedAttr(DBREFLISTXML::REFLIST, propsRefList, QIcon(":/reflist"));

    QStringList propsModel;
    propsModel << DBMODELXML::NAME   << DBMODELXML::ALIAS
               << DBMODELXML::PARENT << DBMODELXML::ID;
    m_model->addDisplayedAttr(DBMODELXML::MODEL, propsModel, QIcon(":/model"));

    QStringList propsLov;
    propsLov << DBLOVXML::NAME   << DBLOVXML::ALIAS
                   << DBLOVXML::PARENT << DBLOVXML::ID
                   << DBLOVXML::TYPE;
    m_model->addDisplayedAttr(DBLOVXML::LOV, propsLov, QIcon(":/lov"));

    QStringList propsLovValue;
    propsLovValue << DBLOVVALUEXML::NAME   << DBLOVVALUEXML::VALUE
                     << DBLOVVALUEXML::PARENT << DBLOVVALUEXML::ID;
    m_model->addDisplayedAttr(DBLOVVALUEXML::LOVVALUE, propsLov, QIcon(":/lovvalue"));

    QStringList propsRefGroup;
    propsRefGroup << DBREFGROUPXML::NAME   << DBREFGROUPXML::DESCRIPTION
                  << DBREFGROUPXML::PARENT << DBREFGROUPXML::ID;
    m_model->addDisplayedAttr(DBREFGROUPXML::REFGROUP,
                              propsRefGroup, QIcon(":/refgroup"));

    QStringList propsRef;
    propsRef << DBREFXML::NAME   << DBREFXML::DESCRIPTION
             << DBREFXML::PARENT << DBREFXML::ID;
    m_model->addDisplayedAttr(DBREFXML::REF, propsRef, QIcon(":/reference"));

    QStringList propsLinkToClass;
    propsLinkToClass << DBLINKTOCLASSXML::ALIAS   << DBLINKTOCLASSXML::REFCLASS
                     << DBLINKTOCLASSXML::PARENT << DBLINKTOCLASSXML::ID;
    m_model->addDisplayedAttr(DBLINKTOCLASSXML::LINKTOCLASS,
                              propsLinkToClass, QIcon(":/linktoclass"));

    QStringList propsLinkToFilter;
    propsLinkToFilter << DBLINKTOFILTERXML::ALIAS   << DBLINKTOFILTERXML::REFFILTER
                      << DBLINKTOFILTERXML::PARENT << DBLINKTOFILTERXML::ID;
    m_model->addDisplayedAttr(DBLINKTOFILTERXML::LINKTOFILTER,
                              propsLinkToFilter, QIcon(":/linktofilter"));

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


    QStringList insertTags;
    insertTags << DBATTRXML::ATTR  << DBCLASSXML::CLASS << DBCOMPXML::COMP
               << DBFILTERXML::FILTER;
    m_model->addInsertTags(DBCLASSXML::CLASS,insertTags);

    insertTags.clear();
    insertTags << DBATTRXML::ATTR;
    m_model->addInsertTags(DBCOMPXML::COMP,insertTags);

    insertTags.clear();
    insertTags << DBFILTERBLOCKXML::BLOCK << DBCONDITIONXML::COND;
    m_model->addInsertTags(DBFILTERXML::FILTER,insertTags);

    insertTags.clear();
    insertTags << DBCONDITIONXML::COND << DBFILTERBLOCKXML::BLOCK;
    m_model->addInsertTags(DBFILTERBLOCKXML::BLOCK,insertTags);

    insertTags.clear();
    insertTags << DBCLASSXML::CLASS;
    m_model->addInsertTags(DBCLASSLISTXML::CLASSLIST,insertTags);

    insertTags.clear();
    insertTags << DBENTITYGROUPXML::ENTITYGROUP;
    m_model->addInsertTags(DBENTITYLISTXML::ENTITYLIST,insertTags);

    insertTags.clear();
    insertTags << DBUNITXML::UNIT;
    m_model->addInsertTags(DBENTITYXML::ENTITY,insertTags);

    insertTags.clear();
    insertTags << DBCLASSLISTXML::CLASSLIST << DBENTITYLISTXML::ENTITYLIST
               << DBLOVLISTXML::LOVLIST << DBREFLISTXML::REFLIST;
    m_model->addInsertTags(DBMODELXML::MODEL,insertTags);

    insertTags.clear();
    insertTags << DBENTITYXML::ENTITY;
    m_model->addInsertTags(DBENTITYGROUPXML::ENTITYGROUP,insertTags);

    insertTags.clear();
    insertTags << DBLOVXML::LOV;
    m_model->addInsertTags(DBLOVLISTXML::LOVLIST,insertTags);

    insertTags.clear();
    insertTags << DBLOVVALUEXML::LOVVALUE;
    m_model->addInsertTags(DBLOVXML::LOV,insertTags);

    insertTags.clear();
    insertTags << DBREFGROUPXML::REFGROUP;
    m_model->addInsertTags(DBREFLISTXML::REFLIST,insertTags);

    insertTags.clear();
    insertTags << DBREFXML::REF;
    m_model->addInsertTags(DBREFGROUPXML::REFGROUP,insertTags);

    insertTags.clear();
    insertTags << DBLINKTOCLASSXML::LINKTOCLASS << DBLINKTOFILTERXML::LINKTOFILTER;
    m_model->addInsertTags(DBREFXML::REF,insertTags);

    insertTags.clear();
    insertTags << DBLINKTOFILTERXML::LINKTOFILTER;
    m_model->addInsertTags(DBLINKTOCLASSXML::LINKTOCLASS,insertTags);

    insertTags.clear();
    insertTags << DBLINKTOCLASSXML::LINKTOCLASS;
    m_model->addInsertTags(DBLINKTOFILTERXML::LINKTOFILTER,insertTags);

    m_model->addHashAttr(DBCLASSXML::CLASS,
                                DBCLASSXML::NAME,
                                TreeXmlHashModel::UniqueRename);
    m_model->addHashAttr(DBATTRXML::ATTR,
                                DBATTRXML::REFCLASS,
                                TreeXmlHashModel::NoUnique);
    m_model->addHashAttr(DBATTRXML::ATTR,
                                DBATTRXML::REFUNIT,
                                TreeXmlHashModel::NoUnique);
    m_model->addHashAttr(DBATTRXML::ATTR,
                                DBATTRXML::REFLOV,
                                TreeXmlHashModel::NoUnique);
    m_model->addHashAttr(DBCOMPXML::COMP,
                                DBCOMPXML::CLASS,
                                TreeXmlHashModel::NoUnique);
    m_model->addHashAttr(DBFILTERXML::FILTER,
                                DBFILTERXML::CLASS,
                                TreeXmlHashModel::NoUnique);
    m_model->addHashAttr(DBENTITYXML::ENTITY,
                                DBENTITYXML::NAME,
                                TreeXmlHashModel::UniqueRename);
    m_model->addHashAttr(DBUNITXML::UNIT,
                                DBUNITXML::NAME,
                                TreeXmlHashModel::UniqueRename);
    m_model->addHashAttr(DBENTITYGROUPXML::ENTITYGROUP,
                                DBENTITYGROUPXML::NAME,
                                TreeXmlHashModel::UniqueRename);
    m_model->addHashAttr(DBLOVXML::LOV,
                                DBLOVXML::NAME,
                                TreeXmlHashModel::UniqueRename);
    m_model->addHashAttr(DBLOVVALUEXML::LOVVALUE,
                                DBLOVVALUEXML::NAME,
                                TreeXmlHashModel::UniqueRename);
    m_model->addHashAttr(DBREFGROUPXML::REFGROUP,
                                DBREFGROUPXML::NAME,
                                TreeXmlHashModel::UniqueRename);
    m_model->addHashAttr(DBREFXML::REF,
                                DBREFXML::NAME,
                                TreeXmlHashModel::UniqueRename);


    m_model->addHashAttr(DBCLASSXML::CLASS,
                                DBCLASSXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBATTRXML::ATTR,
                                DBATTRXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBCOMPXML::COMP,
                                DBCOMPXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBFILTERXML::FILTER,
                                DBFILTERXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBFILTERBLOCKXML::BLOCK,
                                DBFILTERBLOCKXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBCONDITIONXML::COND,
                                DBCONDITIONXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBENTITYXML::ENTITY,
                                DBENTITYXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBUNITXML::UNIT,
                                DBUNITXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBENTITYGROUPXML::ENTITYGROUP,
                                DBENTITYGROUPXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBLOVXML::LOV,
                                DBLOVXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBLOVVALUEXML::LOVVALUE,
                                DBLOVVALUEXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBREFGROUPXML::REFGROUP,
                                DBREFGROUPXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBREFXML::REF,
                                DBREFXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBLINKTOCLASSXML::LINKTOCLASS,
                                DBLINKTOCLASSXML::ID,
                                TreeXmlHashModel::Uuid);
    m_model->addHashAttr(DBLINKTOFILTERXML::LINKTOFILTER,
                                DBLINKTOFILTERXML::ID,
                                TreeXmlHashModel::Uuid);


    m_model->addRelation(DBATTRXML::ATTR,DBATTRXML::REFCLASS,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);
    m_model->addRelation(DBATTRXML::ATTR,DBATTRXML::PARENT,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);
    m_model->addRelation(DBATTRXML::ATTR,DBATTRXML::PARENT,
                               DBCOMPXML::COMP, DBCOMPXML::NAME);
    m_model->addRelation(DBATTRXML::ATTR,DBATTRXML::REFUNIT,
                               DBUNITXML::UNIT, DBUNITXML::NAME);
    m_model->addRelation(DBATTRXML::ATTR,DBATTRXML::REFLOV,
                               DBLOVXML::LOV, DBLOVXML::NAME);

    m_model->addRelation(DBCLASSXML::CLASS, DBCLASSXML::PARENT,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);

    m_model->addRelation(DBCOMPXML::COMP, DBCOMPXML::PARENT,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);
    m_model->addRelation(DBCOMPXML::COMP, DBCOMPXML::CLASS,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);

    m_model->addRelation(DBFILTERXML::FILTER, DBFILTERXML::PARENT,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);
    m_model->addRelation(DBFILTERXML::FILTER, DBFILTERXML::CLASS,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);

    m_model->addRelation(DBCONDITIONXML::COND, DBCONDITIONXML::FIRSTATTR,
                               DBATTRXML::ATTR, DBATTRXML::NAME);
    m_model->addRelation(DBCONDITIONXML::COND, DBCONDITIONXML::SECONDATTR,
                               DBATTRXML::ATTR, DBATTRXML::NAME);

    m_model->addRelation(DBENTITYXML::ENTITY, DBENTITYXML::BASICUNIT,
                               DBUNITXML::UNIT, DBUNITXML::NAME);
    m_model->addRelation(DBENTITYXML::ENTITY, DBENTITYXML::PARENT,
                               DBENTITYGROUPXML::ENTITYGROUP, DBENTITYGROUPXML::NAME);

    m_model->addRelation(DBUNITXML::UNIT, DBUNITXML::PARENT,
                               DBENTITYXML::ENTITY, DBENTITYXML::NAME);

    m_model->addRelation(DBLOVVALUEXML::LOVVALUE, DBLOVVALUEXML::PARENT,
                               DBLOVXML::LOV, DBLOVXML::NAME);

    m_model->addRelation(DBREFXML::REF, DBREFXML::PARENT,
                               DBREFGROUPXML::REFGROUP, DBREFGROUPXML::NAME);

    m_model->addRelation(DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::PARENT,
                               DBREFGROUPXML::REFGROUP, DBREFGROUPXML::NAME);
    m_model->addRelation(DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::PARENT,
                               DBLINKTOFILTERXML::LINKTOFILTER, DBLINKTOFILTERXML::ALIAS);
    m_model->addRelation(DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::REFCLASS,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);

    m_model->addRelation(DBLINKTOFILTERXML::LINKTOFILTER, DBLINKTOFILTERXML::PARENT,
                               DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::ALIAS);
    m_model->addRelation(DBLINKTOFILTERXML::LINKTOFILTER, DBLINKTOFILTERXML::REFFILTER,
                               DBFILTERXML::FILTER, DBFILTERXML::NAME);


    m_model->refreshHashing();

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

    if (indexSource.data(TreeXmlModel::TagRole)==DBCOMPXML::COMP)
        showPropComposition(indexSource);

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

    if (m_model->isAttr(indexSource))
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
        connect(propClass,SIGNAL(editComposition(QModelIndex)),
                this,SLOT(showPropComposition(QModelIndex)));
        connect(propClass,SIGNAL(editFilter(QModelIndex)),
                this,SLOT(showPropFilter(QModelIndex)));
    } else {
        PropClass* propClass = qobject_cast<PropClass*>(subWindow->widget());
        propClass->setCurrentClass(indexSource);
    }
}

void ModelerIDEPlug::showPropComposition(const QModelIndex &indexSource)
{
    if (!indexSource.isValid())
        return;

    if (indexSource.data(TreeXmlModel::TagRole)!=DBCOMPXML::COMP)
        return;

    PluginManager* pluginManager = PluginManager::instance();
    IMainWindow* mainWindow = qobject_cast<IMainWindow*>(
                pluginManager->interfaceObject("IMainWindow"));

    QString classId = this->dataId(indexSource);

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
    QDomDocument document;
    QDomNode node = document.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    document.insertBefore(node, document.firstChild());
    createClassModel(document);


    QModelIndex indexSource = treeClassView->currentIndex();
    QModelIndex lastIndex = m_model->insertLastRows(0,1,indexSource,DBMODELXML::MODEL);
    if (lastIndex.isValid()){
        treeClassView->setCurrentIndex(lastIndex);

        int column = m_model->columnDisplayedAttr(DBMODELXML::MODEL,
                                                  DBMODELXML::NAME);
        m_model->setData(lastIndex.sibling(lastIndex.row(),column),tr("Model"));

        column = m_model->columnDisplayedAttr(DBMODELXML::MODEL,
                                              DBMODELXML::ALIAS);
        m_model->setData(lastIndex.sibling(lastIndex.row(),column),tr("Модель"));

        indexSource = lastIndex;        
        QModelIndex lastIndex = m_model->insertLastRows(0,1,indexSource,DBCLASSLISTXML::CLASSLIST);
        if (lastIndex.isValid()){
            treeClassView->setCurrentIndex(lastIndex);

            int column = m_model->columnDisplayedAttr(DBCLASSLISTXML::CLASSLIST,
                                                      DBCLASSLISTXML::NAME);
            m_model->setData(lastIndex.sibling(lastIndex.row(),column),tr("Classes"));

            column = m_model->columnDisplayedAttr(DBCLASSLISTXML::CLASSLIST,
                                                  DBCLASSLISTXML::ALIAS);
            m_model->setData(lastIndex.sibling(lastIndex.row(),column),tr("Классы"));
        }

        lastIndex = m_model->insertLastRows(0,1,indexSource,DBENTITYLISTXML::ENTITYLIST);
        if (lastIndex.isValid()){
            treeClassView->setCurrentIndex(lastIndex);

            int column = m_model->columnDisplayedAttr(DBENTITYLISTXML::ENTITYLIST,
                                                      DBENTITYLISTXML::NAME);
            m_model->setData(lastIndex.sibling(lastIndex.row(),column), tr("Units"));

            column = m_model->columnDisplayedAttr(DBENTITYLISTXML::ENTITYLIST,
                                                  DBENTITYLISTXML::ALIAS);
            m_model->setData(lastIndex.sibling(lastIndex.row(),column), tr("Единицы измерения"));
        }

        lastIndex = m_model->insertLastRows(0,1,indexSource, DBLOVLISTXML::LOVLIST);
        if (lastIndex.isValid()){
            treeClassView->setCurrentIndex(lastIndex);

            int column = m_model->columnDisplayedAttr(DBLOVLISTXML::LOVLIST,
                                                      DBLOVLISTXML::NAME);
            m_model->setData(lastIndex.sibling(lastIndex.row(),column), tr("List of Value"));

            column = m_model->columnDisplayedAttr(DBLOVLISTXML::LOVLIST,
                                                  DBLOVLISTXML::ALIAS);
            m_model->setData(lastIndex.sibling(lastIndex.row(),column), tr("Список значений"));
        }

        lastIndex = m_model->insertLastRows(0,1,indexSource, DBREFLISTXML::REFLIST);
        if (lastIndex.isValid()){
            treeClassView->setCurrentIndex(lastIndex);

            int column = m_model->columnDisplayedAttr(DBREFLISTXML::REFLIST,
                                                      DBREFLISTXML::NAME);
            m_model->setData(lastIndex.sibling(lastIndex.row(),column), tr("References"));

            column = m_model->columnDisplayedAttr(DBREFLISTXML::REFLIST,
                                                  DBREFLISTXML::ALIAS);
            m_model->setData(lastIndex.sibling(lastIndex.row(),column), tr("Справочники"));
        }
    }
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
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropComposition::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropFilter::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropEntityGroup::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropEntity::"))  != -1
         || subWindow->widget()->objectName().indexOf(QRegExp("^PropLov::"))  != -1
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
