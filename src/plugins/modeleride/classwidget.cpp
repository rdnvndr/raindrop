#include "classwidget.h"
#include "dbxmlstruct.h"
#include <treexmlmodel/xmldelegate.h>
#include <QStringListModel>

ClassWidget::ClassWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_typeClassModel = new QStringListModel();
    m_typeClassModel->setStringList(DBXMLCLASSTYPE);

    lineEditClassParent->setReadOnly(true);

    connect(toolButtonAddClass,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDelClass,SIGNAL(clicked()),this,SLOT(remove()));
    connect(pushButtonPropSave,SIGNAL(clicked()),this,SLOT(submit()));
    connect(pushButtonPropCancel,SIGNAL(clicked()),this,SLOT(revert()));
    connect(toolButtonEditClass,SIGNAL(clicked()),this,SLOT(edit()));
}

ClassWidget::~ClassWidget()
{
    delete m_typeClassModel;
    delete m_mapper;
}

void ClassWidget::setModel(TreeXMLModel *model)
{
    m_model = model;

    m_mapper->setModel(m_model);

    comboBoxClassType->setModel(m_typeClassModel);

    m_mapper->addMapping(lineEditClassName,
                         model->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::NAME));
    m_mapper->addMapping(lineEditClassDesc,
                         model->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::DESCRIPTION));

    m_mapper->addMapping(comboBoxClassType,
                         model->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::TYPE));

    m_mapper->addMapping(lineEditClassParent,
                         model->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::PARENT));

    m_mapper->addMapping(checkBoxAbsClass,
                         model->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::ISABSTARCT));
    m_mapper->addMapping(checkBoxActiveClass,
                         model->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::ISACTIVE));
    m_mapper->addMapping(plainTextEditShowAttr,
                         model->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::TEMPLATE));
}

void ClassWidget::add()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    m_model->setInsTagName(DBCLASSXML::CLASS);
    if (m_model->insertRow(0,srcIndex)) {
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

void ClassWidget::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    m_mapper->revert();
    setCurrent(srcIndex.parent());

    if (!modelData(DBCLASSXML::CLASS,DBCLASSXML::NAME,srcIndex).toString().isEmpty()){
        emit dataRemoved(srcIndex);
        m_model->removeRow(srcIndex.row(),srcIndex.parent());
    }

}


void ClassWidget::setCurrent(QModelIndex index)
{
    if (m_mapper->rootIndex() == index.parent() &&
            index.row() == m_mapper->currentIndex())
        return;

    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);
    edit(false);
    int indexType = comboBoxClassType->findText(modelData(DBCLASSXML::CLASS,
                                                          DBCLASSXML::TYPE,
                                                          index).toString());
    comboBoxClassType->setCurrentIndex(indexType);
    emit currentIndexChanged(index);
}

void ClassWidget::edit(bool flag)
{
    if (groupBoxClass->isEnabled()==flag)
        return;

    if (lineEditClassName->text().isEmpty()){
        toolButtonAddClass->setDisabled(true);
        flag = true;
    }else
        toolButtonAddClass->setEnabled(true);

    groupBoxClass->setEnabled(flag);
    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEditClass->setDisabled(flag);
}

void ClassWidget::submit()
{
    m_mapper->submit();
    removeEmpty();
    edit(false);
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    emit dataChanged(srcIndex);
}

void ClassWidget::revert()
{
    m_mapper->revert();
    removeEmpty();
    edit(false);
}

QVariant ClassWidget::modelData(QString typeName, QString attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->indexDisplayedAttr(
                             typeName,attr)).data();
}

void ClassWidget::removeEmpty()
{
    if (lineEditClassName->text().isEmpty()){
        QModelIndex parentIndex = m_mapper->rootIndex();
        if (parentIndex.isValid()){
            m_model->removeRow(m_mapper->currentIndex(),
                               m_mapper->rootIndex());

            setCurrent(parentIndex);
        }else {
            remove();
            return;
        }
    }
}
