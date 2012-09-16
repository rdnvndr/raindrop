#include "dlgeditattr.h"
#include <QSortFilterProxyModel>
#include "dbxmlstruct.h"
#include <treexmlmodel/xmldelegate.h>

DlgEditAttr::DlgEditAttr(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));

    comboBoxRefClass->setEditable(true);
    comboBoxRefClass->lineEdit()->setReadOnly(true);
    comboBoxType->setEditable(true);
    comboBoxType->lineEdit()->setReadOnly(true);
    spinBoxLength->setDisabled(true);


    m_typeModel = new QStringListModel();
    m_typeModel->setStringList(DBXMLTYPE);

    connect(checkBoxIsLength,SIGNAL(toggled(bool)),this,SLOT(setCheckArray(bool)));
    connect(comboBoxType,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeType(QString)));
    connect(spinBoxUpValue,SIGNAL(valueChanged(double)),this,SLOT(changeUpValue(double)));
    connect(spinBoxDownValue,SIGNAL(valueChanged(double)),this,SLOT(changeDownValue(double)));
}

DlgEditAttr::~DlgEditAttr()
{
    delete m_mapper;
    delete m_typeModel;
}

void DlgEditAttr::setModel(QAbstractItemModel *model)
{
    m_model = model;
    m_mapper->setModel(m_model);
    comboBoxType->setModel(m_typeModel);
    QSortFilterProxyModel* proxyModel = qobject_cast<QSortFilterProxyModel*>(m_model);

    comboBoxRefClass->setModel(proxyModel->sourceModel());

    m_mapper->addMapping(lineEditName,0);
    m_mapper->addMapping(comboBoxType,1);
    m_mapper->addMapping(spinBoxLengthStr,2);
    m_mapper->addMapping(comboBoxRefClass,3);
    m_mapper->addMapping(lineEditInitValue,5);
    m_mapper->addMapping(spinBoxDownValue,6);
    m_mapper->addMapping(spinBoxUpValue,7);
    m_mapper->addMapping(groupBoxIsArray,8);
    m_mapper->addMapping(spinBoxLength,9);
    m_mapper->addMapping(checkBoxIsNull,10);
    m_mapper->addMapping(checkBoxIsUnique,11);
    m_mapper->addMapping(checkBoxCandidatKey,12);
    m_mapper->addMapping(plainTextEdit,13);
}

QAbstractItemModel *DlgEditAttr::model()
{
    return m_model;
}

void DlgEditAttr::setCurrentModelIndex(QModelIndex index)
{
    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);
    changeType(comboBoxType->currentText());
}

void DlgEditAttr::setCheckArray(bool check)
{
    spinBoxLength->setEnabled(check);
    if (check)
        spinBoxLength->setValue(1);
    else
        spinBoxLength->setValue(0);
}

void DlgEditAttr::changeType(QString s)
{
    if (DBXMLTYPE.at(0)==s){
        // String
        spinBoxLengthStr->setEnabled(true);
        comboBoxRefClass->setEnabled(false);
        lineEditInitValue->setEnabled(true);
        spinBoxDownValue->setEnabled(false);
        spinBoxUpValue->setEnabled(false);
    } else if( DBXMLTYPE.at(1)==s){
        //Referenced
        spinBoxLengthStr->setEnabled(false);
        comboBoxRefClass->setEnabled(true);
        lineEditInitValue->setEnabled(false);
        spinBoxDownValue->setEnabled(false);
        spinBoxUpValue->setEnabled(false);
    } else {
        // Double and Int
        spinBoxLengthStr->setEnabled(false);
        comboBoxRefClass->setEnabled(false);
        lineEditInitValue->setEnabled(true);
        spinBoxDownValue->setEnabled(true);
        spinBoxUpValue->setEnabled(true);
    }
}

void DlgEditAttr::changeUpValue(double newValue)
{
    if (spinBoxDownValue->value() >= newValue)
        spinBoxDownValue->setValue(newValue-1);
}

void DlgEditAttr::changeDownValue(double newValue)
{
    if (spinBoxUpValue->value() <= newValue)
        spinBoxUpValue->setValue(newValue+1);
}
