#include "classmodel.h"
#include "dbxmlstruct.h"
#include <treexmlmodel/tagxmlitem.h>

ClassModel::ClassModel(QDomNode document, QObject *parent)
    : TreeXmlHashModel(document, parent)
{
    initTagFilters();
    initDisplayedAttrs();
    initInsertTags();
    initHashAttrs();
    initRelations();

    this->refreshHashing();

    if (document.isNull()) initModel();
}

ClassModel::~ClassModel()
{

}

void ClassModel::initTagFilters()
{
    this->addTagFilter(DBCLASSXML::CLASS);
    this->addTagFilter(DBATTRXML::ATTR);
    this->addTagFilter(DBCOMPXML::COMP);
    this->addTagFilter(DBFILTERXML::FILTER);
    this->addTagFilter(DBFILTERBLOCKXML::BLOCK);
    this->addTagFilter(DBCONDITIONXML::COND);
    this->addTagFilter(DBENTITYXML::ENTITY);
    this->addTagFilter(DBUNITXML::UNIT);
    this->addTagFilter(DBENTITYGROUPXML::ENTITYGROUP);
    this->addTagFilter(DBCLASSLISTXML::CLASSLIST);
    this->addTagFilter(DBENTITYLISTXML::ENTITYLIST);
    this->addTagFilter(DBMODELXML::MODEL);
    this->addTagFilter(DBLOVLISTXML::LOVLIST);
    this->addTagFilter(DBLOVXML::LOV);
    this->addTagFilter(DBLOVVALUEXML::LOVVALUE);
    this->addTagFilter(DBREFLISTXML::REFLIST);
    this->addTagFilter(DBREFGROUPXML::REFGROUP);
    this->addTagFilter(DBREFXML::REF);
    this->addTagFilter(DBLINKTOCLASSXML::LINKTOCLASS);
    this->addTagFilter(DBLINKTOFILTERXML::LINKTOFILTER);
    this->addTagFilter(DBLINKTOCOMPXML::LINKTOCOMP);
}

void ClassModel::initDisplayedAttrs()
{
    QStringList propsClass;
    propsClass << DBCLASSXML::NAME      << DBCLASSXML::ISABSTARCT
               << DBCLASSXML::TYPE      << DBCLASSXML::ALIAS
               << DBCLASSXML::PARENT    << DBCLASSXML::ISACTIVE
               << DBCLASSXML::ISCONTEXT << DBCLASSXML::TEMPLATE
               << DBCLASSXML::ID        << DBCLASSXML::ICON;
    this->addDisplayedAttr(DBCLASSXML::CLASS,propsClass,QIcon(":/modeleride"));

    QStringList propsAttr;
    propsAttr << DBATTRXML::NAME           << DBATTRXML::ALIAS
              << DBATTRXML::TYPE           << DBATTRXML::MAXSTRLEN
              << DBATTRXML::REFCLASS       << DBATTRXML::PARENT
              << DBATTRXML::REFUNIT        << DBATTRXML::INITIALVAL
              << DBATTRXML::LOWERBOUND     << DBATTRXML::UPPERBOUND
              << DBATTRXML::REFLOV         << DBATTRXML::GROUP
              << DBATTRXML::ISNULLALLOWED  << DBATTRXML::ISUNIQUE
              << DBATTRXML::ISCANDIDATEKEY << DBATTRXML::ID;
    this->addDisplayedAttr(DBATTRXML::ATTR,propsAttr, QIcon(":/attribute"));
    this->addAttrTag(DBCLASSXML::CLASS, DBATTRXML::ATTR);

    QStringList propsComposition;
    propsComposition << DBCOMPXML::LINKCLASS         << DBCOMPXML::ALIAS
                     << DBCOMPXML::PARENT            << DBCOMPXML::CLASS
                     << DBCOMPXML::ISVIEW            << DBCOMPXML::ISCOMP
                     << DBCOMPXML::DIRECTDESCRIPTION << DBCOMPXML::INVERSEDESCRIPTION
                     << DBCOMPXML::ID;
    this->addDisplayedAttr(DBCOMPXML::COMP,propsComposition, QIcon(":/composition"));
    this->addAttrTag(DBCLASSXML::CLASS, DBCOMPXML::COMP);

    QStringList propsFilter;
    propsFilter << DBFILTERXML::NAME              << DBFILTERXML::ALIAS
                << DBFILTERXML::PARENT            << DBFILTERXML::CLASS
                << DBFILTERXML::DIRECTDESCRIPTION << DBFILTERXML::INVERSEDESCRIPTION
                << DBFILTERXML::ID;
    this->addDisplayedAttr(DBFILTERXML::FILTER,propsFilter, QIcon(":/filter"));
    this->addAttrTag(DBCLASSXML::CLASS, DBFILTERXML::FILTER);

    QStringList propsFilterBlock;
    propsFilterBlock << DBFILTERBLOCKXML::LINKOF << DBFILTERBLOCKXML::PARENT
                     << DBFILTERBLOCKXML::ID;
    this->addDisplayedAttr(DBFILTERBLOCKXML::BLOCK, propsFilterBlock,
                              QIcon(":/block"));

    QStringList propsCondition;
    propsCondition   << DBCONDITIONXML::FIRSTATTR   << DBCONDITIONXML::OPERATOR
                     << DBCONDITIONXML::SECONDATTR  << DBCONDITIONXML::LINKOF
                     << DBCONDITIONXML::PARENT      << DBCONDITIONXML::ID;
    this->addDisplayedAttr(DBCONDITIONXML::COND, propsCondition,
                              QIcon(":/expression"));

    QStringList propsEntity;
    propsEntity << DBENTITYXML::NAME            << DBENTITYXML::DESCRIPTION
                << DBENTITYXML::DIMENSIONSYMBOL << DBENTITYXML::BASICUNIT
                << DBENTITYXML::ID;
    this->addDisplayedAttr(DBENTITYXML::ENTITY,propsEntity, QIcon(":/entity"));

    QStringList propsUnit;
    propsUnit << DBUNITXML::NAME           << DBUNITXML::CODE
              << DBUNITXML::COEFF          << DBUNITXML::DELTA
              << DBUNITXML::DESIGNATION    << DBUNITXML::SYMBOL
              << DBUNITXML::INTDESIGNATION << DBUNITXML::INTSYMBOL
              << DBUNITXML::PARENT         << DBUNITXML::ID;
    this->addDisplayedAttr(DBUNITXML::UNIT,propsUnit, QIcon(":/unit"));

    QStringList propsGroup;
    propsGroup << DBENTITYGROUPXML::NAME   << DBENTITYGROUPXML::DESCRIPTION
               << DBENTITYGROUPXML::PARENT << DBENTITYGROUPXML::ID;
    this->addDisplayedAttr(DBENTITYGROUPXML::ENTITYGROUP,propsGroup, QIcon(":/entitygroup"));

    QStringList propsClassList;
    propsClassList << DBCLASSLISTXML::NAME   << DBCLASSLISTXML::ALIAS
                   << DBCLASSLISTXML::PARENT << DBCLASSLISTXML::ID;
    this->addDisplayedAttr(DBCLASSLISTXML::CLASSLIST,propsClassList, QIcon(":/classes"));

    QStringList propsEntityList;
    propsEntityList << DBENTITYLISTXML::NAME   << DBENTITYLISTXML::ALIAS
                    << DBENTITYLISTXML::PARENT << DBENTITYLISTXML::ID;
    this->addDisplayedAttr(DBENTITYLISTXML::ENTITYLIST, propsEntityList, QIcon(":/units"));

    QStringList propsLovList;
    propsLovList << DBLOVLISTXML::NAME   << DBLOVLISTXML::ALIAS
                 << DBLOVLISTXML::PARENT << DBLOVLISTXML::ID;
    this->addDisplayedAttr(DBLOVLISTXML::LOVLIST, propsLovList, QIcon(":/lovlist"));

    QStringList propsRefList;
    propsRefList << DBREFLISTXML::NAME   << DBREFLISTXML::ALIAS
                 << DBREFLISTXML::PARENT << DBREFLISTXML::ID;
    this->addDisplayedAttr(DBREFLISTXML::REFLIST, propsRefList, QIcon(":/reflist"));

    QStringList propsModel;
    propsModel << DBMODELXML::NAME   << DBMODELXML::ALIAS
               << DBMODELXML::PARENT << DBMODELXML::ID;
    this->addDisplayedAttr(DBMODELXML::MODEL, propsModel, QIcon(":/model"));

    QStringList propsLov;
    propsLov << DBLOVXML::NAME   << DBLOVXML::ALIAS
             << DBLOVXML::PARENT << DBLOVXML::ID
             << DBLOVXML::TYPE;
    this->addDisplayedAttr(DBLOVXML::LOV, propsLov, QIcon(":/lov"));

    QStringList propsLovValue;
    propsLovValue << DBLOVVALUEXML::NAME   << DBLOVVALUEXML::VALUE
                  << DBLOVVALUEXML::PARENT << DBLOVVALUEXML::ID;
    this->addDisplayedAttr(DBLOVVALUEXML::LOVVALUE, propsLov, QIcon(":/lovvalue"));

    QStringList propsRefGroup;
    propsRefGroup << DBREFGROUPXML::NAME   << DBREFGROUPXML::DESCRIPTION
                  << DBREFGROUPXML::PARENT << DBREFGROUPXML::ID;
    this->addDisplayedAttr(DBREFGROUPXML::REFGROUP,
                              propsRefGroup, QIcon(":/refgroup"));

    QStringList propsRef;
    propsRef << DBREFXML::NAME   << DBREFXML::DESCRIPTION
             << DBREFXML::PARENT << DBREFXML::ID;
    this->addDisplayedAttr(DBREFXML::REF, propsRef, QIcon(":/reference"));

    QStringList propsLinkToClass;
    propsLinkToClass << DBLINKTOCLASSXML::ALIAS  << DBLINKTOCLASSXML::REFCLASS
                     << DBLINKTOCLASSXML::PARENT << DBLINKTOCLASSXML::ID;
    this->addDisplayedAttr(DBLINKTOCLASSXML::LINKTOCLASS,
                              propsLinkToClass, QIcon(":/modeleride"));

    QStringList propsLinkToFilter;
    propsLinkToFilter << DBLINKTOFILTERXML::ALIAS   << DBLINKTOFILTERXML::REFFILTER
                      << DBLINKTOFILTERXML::PARENT << DBLINKTOFILTERXML::ID;
    this->addDisplayedAttr(DBLINKTOFILTERXML::LINKTOFILTER,
                              propsLinkToFilter, QIcon(":/filter"));

    QStringList propsLinkToComp;
    propsLinkToComp << DBLINKTOCOMPXML::ALIAS  << DBLINKTOCOMPXML::REFCOMP
                    << DBLINKTOCOMPXML::PARENT << DBLINKTOCOMPXML::ID;
    this->addDisplayedAttr(DBLINKTOCOMPXML::LINKTOCOMP,
                           propsLinkToComp, QIcon(":/composition"));
}

void ClassModel::initInsertTags()
{
    QStringList insertTags;

    insertTags << DBATTRXML::ATTR  << DBCLASSXML::CLASS << DBCOMPXML::COMP
               << DBFILTERXML::FILTER;
    this->addInsertTags(DBCLASSXML::CLASS,insertTags);

    insertTags.clear();
    insertTags << DBATTRXML::ATTR;
    this->addInsertTags(DBCOMPXML::COMP,insertTags);

    insertTags.clear();
    insertTags << DBFILTERBLOCKXML::BLOCK << DBCONDITIONXML::COND;
    this->addInsertTags(DBFILTERXML::FILTER,insertTags);

    insertTags.clear();
    insertTags << DBCONDITIONXML::COND << DBFILTERBLOCKXML::BLOCK;
    this->addInsertTags(DBFILTERBLOCKXML::BLOCK,insertTags);

    insertTags.clear();
    insertTags << DBCLASSXML::CLASS;
    this->addInsertTags(DBCLASSLISTXML::CLASSLIST,insertTags);

    insertTags.clear();
    insertTags << DBENTITYGROUPXML::ENTITYGROUP;
    this->addInsertTags(DBENTITYLISTXML::ENTITYLIST,insertTags);

    insertTags.clear();
    insertTags << DBUNITXML::UNIT;
    this->addInsertTags(DBENTITYXML::ENTITY,insertTags);

    insertTags.clear();
    insertTags << DBCLASSLISTXML::CLASSLIST << DBENTITYLISTXML::ENTITYLIST
               << DBLOVLISTXML::LOVLIST     << DBREFLISTXML::REFLIST;
    this->addInsertTags(DBMODELXML::MODEL,insertTags);

    insertTags.clear();
    insertTags << DBENTITYXML::ENTITY;
    this->addInsertTags(DBENTITYGROUPXML::ENTITYGROUP,insertTags);

    insertTags.clear();
    insertTags << DBLOVXML::LOV;
    this->addInsertTags(DBLOVLISTXML::LOVLIST,insertTags);

    insertTags.clear();
    insertTags << DBLOVVALUEXML::LOVVALUE;
    this->addInsertTags(DBLOVXML::LOV,insertTags);

    insertTags.clear();
    insertTags << DBREFGROUPXML::REFGROUP;
    this->addInsertTags(DBREFLISTXML::REFLIST,insertTags);

    insertTags.clear();
    insertTags << DBREFXML::REF;
    this->addInsertTags(DBREFGROUPXML::REFGROUP,insertTags);

    insertTags.clear();
    insertTags << DBLINKTOCLASSXML::LINKTOCLASS << DBLINKTOFILTERXML::LINKTOFILTER;
    this->addInsertTags(DBREFXML::REF,insertTags);

    insertTags.clear();
    insertTags << DBLINKTOFILTERXML::LINKTOFILTER << DBLINKTOCOMPXML::LINKTOCOMP;
    this->addInsertTags(DBLINKTOCLASSXML::LINKTOCLASS,insertTags);

    insertTags.clear();
    insertTags << DBLINKTOCLASSXML::LINKTOCLASS;
    this->addInsertTags(DBLINKTOFILTERXML::LINKTOFILTER,insertTags);

    insertTags.clear();
    insertTags << DBLINKTOCLASSXML::LINKTOCLASS;
    this->addInsertTags(DBLINKTOCOMPXML::LINKTOCOMP,insertTags);
}

void ClassModel::initHashAttrs()
{
    this->addHashAttr(DBCLASSXML::CLASS,
                                DBCLASSXML::NAME,
                                TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBATTRXML::ATTR,
                                DBATTRXML::NAME,
                                TreeXmlHashModel::UniqueParentUpperRename);
    this->addHashAttr(DBATTRXML::ATTR,
                                DBATTRXML::REFCLASS,
                                TreeXmlHashModel::NoUnique);
    this->addHashAttr(DBATTRXML::ATTR,
                                DBATTRXML::REFUNIT,
                                TreeXmlHashModel::NoUnique);
    this->addHashAttr(DBATTRXML::ATTR,
                                DBATTRXML::REFLOV,
                                TreeXmlHashModel::NoUnique);
    this->addHashAttr(DBCOMPXML::COMP,
                                DBCOMPXML::CLASS,
                                TreeXmlHashModel::NoUnique);
    this->addHashAttr(DBCOMPXML::COMP,
                                DBCOMPXML::LINKCLASS,
                                TreeXmlHashModel::UniqueParent);
    this->addHashAttr(DBFILTERXML::FILTER,
                                DBFILTERXML::CLASS,
                                TreeXmlHashModel::NoUnique);
    this->addHashAttr(DBFILTERXML::FILTER,
                                DBFILTERXML::NAME,
                                TreeXmlHashModel::UniqueParentUpperRename);
    this->addHashAttr(DBENTITYXML::ENTITY,
                                DBENTITYXML::NAME,
                                TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBUNITXML::UNIT,
                                DBUNITXML::NAME,
                                TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBENTITYGROUPXML::ENTITYGROUP,
                                DBENTITYGROUPXML::NAME,
                                TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBLOVXML::LOV,
                                DBLOVXML::NAME,
                                TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBLOVVALUEXML::LOVVALUE,
                                DBLOVVALUEXML::NAME,
                                TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBREFGROUPXML::REFGROUP,
                                DBREFGROUPXML::NAME,
                                TreeXmlHashModel::UniqueUpperRename);
    this->addHashAttr(DBREFXML::REF,
                                DBREFXML::NAME,
                                TreeXmlHashModel::UniqueUpperRename);


    this->addHashAttr(DBCLASSXML::CLASS,
                                DBCLASSXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBATTRXML::ATTR,
                                DBATTRXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBCOMPXML::COMP,
                                DBCOMPXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBFILTERXML::FILTER,
                                DBFILTERXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBFILTERBLOCKXML::BLOCK,
                                DBFILTERBLOCKXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBCONDITIONXML::COND,
                                DBCONDITIONXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBENTITYXML::ENTITY,
                                DBENTITYXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBUNITXML::UNIT,
                                DBUNITXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBENTITYGROUPXML::ENTITYGROUP,
                                DBENTITYGROUPXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBLOVXML::LOV,
                                DBLOVXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBLOVVALUEXML::LOVVALUE,
                                DBLOVVALUEXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBREFGROUPXML::REFGROUP,
                                DBREFGROUPXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBREFXML::REF,
                                DBREFXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBLINKTOCLASSXML::LINKTOCLASS,
                                DBLINKTOCLASSXML::ID,
                                TreeXmlHashModel::Uuid);
    this->addHashAttr(DBLINKTOFILTERXML::LINKTOFILTER,
                                DBLINKTOFILTERXML::ID,
                         TreeXmlHashModel::Uuid);
    this->addHashAttr(DBLINKTOCOMPXML::LINKTOCOMP,
                                DBLINKTOCOMPXML::ID,
                         TreeXmlHashModel::Uuid);
}

void ClassModel::initRelations()
{
    this->addRelation(DBATTRXML::ATTR,DBATTRXML::REFCLASS,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);
    this->addRelation(DBATTRXML::ATTR,DBATTRXML::PARENT,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);
    this->addRelation(DBATTRXML::ATTR,DBATTRXML::PARENT,
                               DBCOMPXML::COMP, DBCOMPXML::NAME);
    this->addRelation(DBATTRXML::ATTR,DBATTRXML::REFUNIT,
                               DBUNITXML::UNIT, DBUNITXML::NAME);
    this->addRelation(DBATTRXML::ATTR,DBATTRXML::REFLOV,
                               DBLOVXML::LOV, DBLOVXML::NAME);

    this->addRelation(DBCLASSXML::CLASS, DBCLASSXML::PARENT,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);

    this->addRelation(DBCOMPXML::COMP, DBCOMPXML::PARENT,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);
    this->addRelation(DBCOMPXML::COMP, DBCOMPXML::CLASS,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);
    this->addRelation(DBCOMPXML::COMP, DBCOMPXML::LINKCLASS,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);

    this->addRelation(DBFILTERXML::FILTER, DBFILTERXML::PARENT,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);
    this->addRelation(DBFILTERXML::FILTER, DBFILTERXML::CLASS,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);

    this->addRelation(DBCONDITIONXML::COND, DBCONDITIONXML::FIRSTATTR,
                               DBATTRXML::ATTR, DBATTRXML::NAME);
    this->addRelation(DBCONDITIONXML::COND, DBCONDITIONXML::SECONDATTR,
                               DBATTRXML::ATTR, DBATTRXML::NAME);

    this->addRelation(DBENTITYXML::ENTITY, DBENTITYXML::BASICUNIT,
                               DBUNITXML::UNIT, DBUNITXML::NAME);
    this->addRelation(DBENTITYXML::ENTITY, DBENTITYXML::PARENT,
                               DBENTITYGROUPXML::ENTITYGROUP, DBENTITYGROUPXML::NAME);

    this->addRelation(DBUNITXML::UNIT, DBUNITXML::PARENT,
                               DBENTITYXML::ENTITY, DBENTITYXML::NAME);

    this->addRelation(DBLOVVALUEXML::LOVVALUE, DBLOVVALUEXML::PARENT,
                               DBLOVXML::LOV, DBLOVXML::NAME);

    this->addRelation(DBREFXML::REF, DBREFXML::PARENT,
                               DBREFGROUPXML::REFGROUP, DBREFGROUPXML::NAME);

    this->addRelation(DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::PARENT,
                               DBREFGROUPXML::REFGROUP, DBREFGROUPXML::NAME);
    this->addRelation(DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::PARENT,
                               DBLINKTOFILTERXML::LINKTOFILTER, DBLINKTOFILTERXML::ALIAS);
    this->addRelation(DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::PARENT,
                               DBLINKTOCOMPXML::LINKTOCOMP, DBLINKTOCOMPXML::ALIAS);
    this->addRelation(DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::REFCLASS,
                               DBCLASSXML::CLASS, DBCLASSXML::NAME);

    this->addRelation(DBLINKTOFILTERXML::LINKTOFILTER, DBLINKTOFILTERXML::PARENT,
                               DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::ALIAS);
    this->addRelation(DBLINKTOFILTERXML::LINKTOFILTER, DBLINKTOFILTERXML::REFFILTER,
                      DBFILTERXML::FILTER, DBFILTERXML::NAME);

    this->addRelation(DBLINKTOCOMPXML::LINKTOCOMP, DBLINKTOCOMPXML::PARENT,
                               DBLINKTOCLASSXML::LINKTOCLASS, DBLINKTOCLASSXML::ALIAS);
    this->addRelation(DBLINKTOCOMPXML::LINKTOCOMP, DBLINKTOCOMPXML::REFCOMP,
                      DBCOMPXML::COMP, DBCOMPXML::LINKCLASS);
}

void ClassModel::initModel()
{
    QModelIndex indexSource = this->index(-1,-1);
    QModelIndex lastIndex = this->insertLastRows(0,1,indexSource,DBMODELXML::MODEL);
    if (lastIndex.isValid()){
        int column = this->columnDisplayedAttr(DBMODELXML::MODEL,
                                                  DBMODELXML::NAME);
        this->setData(lastIndex.sibling(lastIndex.row(),column),tr("Model"));
        column = this->columnDisplayedAttr(DBMODELXML::MODEL,
                                              DBMODELXML::ALIAS);
        this->setData(lastIndex.sibling(lastIndex.row(),column),tr("Модель"));

        indexSource = lastIndex;
        QModelIndex lastIndex = this->insertLastRows(0,1,indexSource,DBCLASSLISTXML::CLASSLIST);
        if (lastIndex.isValid()){
            int column = this->columnDisplayedAttr(DBCLASSLISTXML::CLASSLIST,
                                                      DBCLASSLISTXML::NAME);
            this->setData(lastIndex.sibling(lastIndex.row(),column),tr("Classes"));
            column = this->columnDisplayedAttr(DBCLASSLISTXML::CLASSLIST,
                                                  DBCLASSLISTXML::ALIAS);
            this->setData(lastIndex.sibling(lastIndex.row(),column),tr("Классы"));
        }

        lastIndex = this->insertLastRows(0,1,indexSource,DBENTITYLISTXML::ENTITYLIST);
        if (lastIndex.isValid()){
            int column = this->columnDisplayedAttr(DBENTITYLISTXML::ENTITYLIST,
                                                      DBENTITYLISTXML::NAME);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("Units"));
            column = this->columnDisplayedAttr(DBENTITYLISTXML::ENTITYLIST,
                                                  DBENTITYLISTXML::ALIAS);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("Единицы измерения"));
        }

        lastIndex = this->insertLastRows(0,1,indexSource, DBLOVLISTXML::LOVLIST);
        if (lastIndex.isValid()){
            int column = this->columnDisplayedAttr(DBLOVLISTXML::LOVLIST,
                                                      DBLOVLISTXML::NAME);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("List of Value"));
            column = this->columnDisplayedAttr(DBLOVLISTXML::LOVLIST,
                                                  DBLOVLISTXML::ALIAS);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("Список значений"));
        }

        lastIndex = this->insertLastRows(0,1,indexSource, DBREFLISTXML::REFLIST);
        if (lastIndex.isValid()){
            int column = this->columnDisplayedAttr(DBREFLISTXML::REFLIST,
                                                      DBREFLISTXML::NAME);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("References"));

            column = this->columnDisplayedAttr(DBREFLISTXML::REFLIST,
                                                  DBREFLISTXML::ALIAS);
            this->setData(lastIndex.sibling(lastIndex.row(),column), tr("Справочники"));
        }
    }
}

