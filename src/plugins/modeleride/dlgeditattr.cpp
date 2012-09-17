#include "dlgeditattr.h"
#include <QSortFilterProxyModel>
#include "dbxmlstruct.h"
#include <treexmlmodel/xmldelegate.h>
#include <treexmlmodel/treexmlmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>

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

    TableXMLProxyModel* tableModel = qobject_cast<TableXMLProxyModel*>(model);
    TreeXMLModel *treeModel = qobject_cast<TreeXMLModel*>(tableModel->sourceModel());
    m_mapper->addMapping(lineEditName,
                         treeModel->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::NAME));
    m_mapper->addMapping(comboBoxType,
                         treeModel->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::TYPE));
    m_mapper->addMapping(spinBoxLengthStr,
                         treeModel->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::MAXSTRLEN));
    m_mapper->addMapping(comboBoxRefClass,
                         treeModel->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::REFCLASS));
    m_mapper->addMapping(lineEditInitValue,
                         treeModel->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::INITIALVAL));
    m_mapper->addMapping(spinBoxDownValue,
                         treeModel->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::LOWVAL));
    m_mapper->addMapping(spinBoxUpValue,
                         treeModel->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::UPVAL));
    m_mapper->addMapping(spinBoxLength,
                         treeModel->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::PARENT));
    m_mapper->addMapping(checkBoxIsNull,
                         treeModel->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::ISNULLALLOWED));
    m_mapper->addMapping(checkBoxIsUnique,
                         treeModel->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::ISUNIQUE));
    m_mapper->addMapping(checkBoxCandidatKey,
                         treeModel->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::ISCANDIDATEKEY));
    m_mapper->addMapping(plainTextEdit,
                         treeModel->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::DESCRIPTION));
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
    if (DBXMLTYPE.at(6)==s){
        // String
        spinBoxLengthStr->setEnabled(true);
        comboBoxRefClass->setEnabled(false);
        lineEditInitValue->setEnabled(true);
        spinBoxDownValue->setEnabled(false);
        spinBoxUpValue->setEnabled(false);
    } else if( DBXMLTYPE.at(7)==s){
        //Reference
        spinBoxLengthStr->setEnabled(false);
        comboBoxRefClass->setEnabled(true);
        lineEditInitValue->setEnabled(false);
        spinBoxDownValue->setEnabled(false);
        spinBoxUpValue->setEnabled(false);
    } else if( DBXMLTYPE.at(4)==s || DBXMLTYPE.at(5)==s){
        // Double and Int
        spinBoxLengthStr->setEnabled(false);
        comboBoxRefClass->setEnabled(false);
        lineEditInitValue->setEnabled(true);
        spinBoxDownValue->setEnabled(true);
        spinBoxUpValue->setEnabled(true);
    } else {
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
