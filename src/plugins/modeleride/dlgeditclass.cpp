#include "dlgeditclass.h"

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
    m_mapper->addMapping(lineEditNameClass,0);
    m_mapper->addMapping(checkBoxAbstract,1);
    m_mapper->addMapping(checkBoxSystem,2);
    m_mapper->addMapping(plainTextEditDesc,3);
    m_mapper->addMapping(lineEditParentClass,4);
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
