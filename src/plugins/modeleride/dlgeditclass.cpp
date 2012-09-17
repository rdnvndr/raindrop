#include "dlgeditclass.h"
#include <treexmlmodel/treexmlmodel.h>
#include "dbxmlstruct.h"

DlgEditClass::DlgEditClass(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    m_mapper = new QDataWidgetMapper();
    connect(buttonBox,SIGNAL(accepted()),m_mapper,SLOT(submit()));
}

DlgEditClass::~DlgEditClass()
{
    delete m_mapper;
}

void DlgEditClass::setModel(QAbstractItemModel *model)
{
    m_model = model;
    m_mapper->setModel(m_model);
    TreeXMLModel *treeModel = qobject_cast<TreeXMLModel*>(model);
    m_mapper->addMapping(lineEditNameClass,
                         treeModel->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                       DBCLASSXML::NAME));
    m_mapper->addMapping(checkBoxAbstract,
                         treeModel->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                       DBCLASSXML::ISABSTARCT));
    m_mapper->addMapping(checkBoxSystem,
                         treeModel->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                       DBCLASSXML::ISACTIVE));
    m_mapper->addMapping(plainTextEditDesc,
                         treeModel->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                       DBCLASSXML::DESCRIPTION));
    m_mapper->addMapping(lineEditParentClass,
                         treeModel->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                       DBCLASSXML::PARENT));
    /*m_mapper->addMapping(lineEditParentClass,
                         treeModel->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                       DBCLASSXML::TYPE));*/
}


QAbstractItemModel *DlgEditClass::model()
{
    return m_model;
}

void DlgEditClass::setCurrentModelIndex(QModelIndex index)
{
    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);
}
