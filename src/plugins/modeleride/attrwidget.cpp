#include "attrwidget.h"
#include "dbxmlstruct.h"
#include <treexmlmodel/xmldelegate.h>
#include <QStringListModel>

AttrWidget::AttrWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_mapperAttr = new QDataWidgetMapper();
    m_mapperAttr->setItemDelegate(new XmlDelegate(this));
    m_mapperAttr->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_attrModel = new TableXMLProxyModel();
    QStringList tags;
    tags << DBATTRXML::ATTR;
    m_attrModel->setAttributeTags(tags);

    m_typeAttrModel = new QStringListModel();
    m_typeAttrModel->setStringList(DBXMLATTRTYPE);

    tableViewAttr->setItemDelegate(new XmlDelegate(this));
    tableViewAttr->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(tableViewAttr,SIGNAL(clicked(QModelIndex)),
            this,SLOT(setCurrent(QModelIndex)));
    connect(toolButtonAddAttr,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDeleteAttr,SIGNAL(clicked()),this,SLOT(remove()));

    connect(comboBoxTypeAttr,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeType(QString)));
    connect(pushButtonAttrSave,SIGNAL(clicked()),this,SLOT(submit()));
    connect(pushButtonAttrCancel,SIGNAL(clicked()),this,SLOT(revert()));
    connect(toolButtonEditAttr,SIGNAL(clicked()),this,SLOT(edit()));
    connect(checkBoxInInherited,SIGNAL(clicked(bool)),this,SLOT(showParentAttr(bool)));
}

AttrWidget::~AttrWidget()
{
    delete m_typeAttrModel;
    delete m_mapperAttr;
    delete m_attrModel;
}

void AttrWidget::setModel(TreeXMLModel *model)
{
    m_model = model;

    m_attrModel->setSourceModel(m_model);
    tableViewAttr->setModel(m_attrModel);

    m_mapperAttr->setModel(m_attrModel);

    comboBoxLinkAttr->setModel(m_model);
    comboBoxTypeAttr->setModel(m_typeAttrModel);

    m_mapperAttr->addMapping(lineEditAttrName,
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::NAME));
    m_mapperAttr->addMapping(comboBoxTypeAttr,
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::TYPE));
    m_mapperAttr->addMapping(spinBoxStringLen,
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::MAXSTRLEN));
    m_mapperAttr->addMapping(comboBoxLinkAttr,
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::REFCLASS));
    m_mapperAttr->addMapping(lineEditDefaultValue,
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::INITIALVAL));
    m_mapperAttr->addMapping(checkBoxIsNull,
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::ISNULLALLOWED));
    m_mapperAttr->addMapping(checkBoxIsUnique,
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::ISUNIQUE));
    m_mapperAttr->addMapping(checkBoxIsCandidateKey,
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::ISCANDIDATEKEY));
    m_mapperAttr->addMapping(lineEditAttrDesc,
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::DESCRIPTION));
    m_mapperAttr->addMapping(comboBoxAttrGroup,
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::GROUP));
}

void AttrWidget::setRootIndex(QModelIndex index)
{
    QModelIndex rootIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    if (rootIndex == index)
        return;

    m_attrModel->setFilterIndex(index);
    for (int row=0;row<comboBoxAttrGroup->count();row++)
        comboBoxAttrGroup->removeItem(row);

    for (int row=0;row<m_attrModel->rowCount(m_attrModel->mapFromSource(index));row++){
        QString insText = m_attrModel->mapFromSource(index).child(
                    row,m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                    DBATTRXML::GROUP)).data().toString();
        if (comboBoxAttrGroup->findText(insText)==-1)
            comboBoxAttrGroup->addItem(insText);
    }

    m_attrModel->setSourceModel(m_model);
    tableViewAttr->setRootIndex(m_attrModel->mapFromSource(index));
    tableViewAttr->setCurrentIndex(tableViewAttr->rootIndex().child(0,0));

    this->setCurrent(tableViewAttr->rootIndex().child(0,0));
}

void AttrWidget::add()
{
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    m_model->setInsTagName(DBATTRXML::ATTR);
    if (m_model->insertRow(0,srcIndex)){
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        this->setCurrent(m_attrModel->mapFromSource(srcCurrentIndex));
        comboBoxTypeAttr->setCurrentIndex(5);
        edit(true);
    }
}

void AttrWidget::remove()
{
    edit(false);
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    QModelIndex curIndex = m_attrModel->mapToSource(tableViewAttr->currentIndex());
    if (srcIndex.isValid() && curIndex.isValid()){
        emit dataRemoved(srcIndex);
        m_model->removeRow(curIndex.row(),srcIndex);
        this->setCurrent(tableViewAttr->currentIndex());
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить атрибут, поскольку нет выбраных атрибутов."));
}


void AttrWidget::submit()
{
    edit(false);
    m_mapperAttr->submit();
    QModelIndex srcIndex = m_mapperAttr->rootIndex().child(m_mapperAttr->currentIndex(),0);
    emit dataChanged(srcIndex);
}

void AttrWidget::edit(bool flag)
{
    if (groupBoxPropAttr->isEnabled()==flag || !tableViewAttr->currentIndex().isValid())
        return;

    if (lineEditAttrName->text().isEmpty() && flag==false)
        m_attrModel->removeRow(m_mapperAttr->currentIndex(),
                               m_mapperAttr->rootIndex());

    groupBoxPropAttr->setEnabled(flag);
    groupBoxPropType->setEnabled(flag);
    pushButtonAttrSave->setEnabled(flag);
    pushButtonAttrCancel->setEnabled(flag);
    toolButtonEditAttr->setDisabled(flag);
}

void AttrWidget::revert()
{
    m_mapperAttr->revert();
    edit(false);
}

void AttrWidget::showParentAttr(bool flag)
{
    m_attrModel->setFilterRole(Qt::EditRole);

    if (flag==true){
        m_attrModel->setFilterRegExp("");
    } else {
        QModelIndex index = m_mapperAttr->rootIndex();
        QString className = modelData(DBCLASSXML::CLASS,
                                      DBCLASSXML::NAME,
                                      index).toString();
        if (className.isEmpty()){
            m_attrModel->setFilterRegExp("\\S*");
        }else
            m_attrModel->setFilterRegExp(className);
    }
    m_attrModel->setFilterKeyColumn(m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                                DBATTRXML::PARENT));
}

void AttrWidget::changeType(QString s)
{
    if (DBXMLATTRTYPE.at(6)==s){
        // String
        spinBoxStringLen->setEnabled(true);
        comboBoxLinkAttr->setEnabled(false);
        lineEditDefaultValue->setEnabled(true);
    } else if( DBXMLATTRTYPE.at(7)==s){
        //Reference
        spinBoxStringLen->setEnabled(false);
        comboBoxLinkAttr->setEnabled(true);
        lineEditDefaultValue->setEnabled(false);
    } else {
        spinBoxStringLen->setEnabled(false);
        comboBoxLinkAttr->setEnabled(false);
        lineEditDefaultValue->setEnabled(true);
    }
}

QVariant AttrWidget::modelData(QString typeName, QString attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->indexDisplayedAttr(
                             typeName,attr)).data();
}


void AttrWidget::setCurrent(QModelIndex index)
{
    if (!index.isValid())
        return;

    if (m_mapperAttr->rootIndex() == index.parent() &&
            index.row() == m_mapperAttr->currentIndex())
        return;

    edit(false);
    m_mapperAttr->setRootIndex(index.parent());
    m_mapperAttr->setCurrentModelIndex(index);
    tableViewAttr->setCurrentIndex(index);

    int indexType = comboBoxTypeAttr->findText(modelData(
                                                   DBATTRXML::ATTR,
                                                   DBATTRXML::TYPE,
                                                   index).toString());
    comboBoxTypeAttr->setCurrentIndex(indexType);

    int indexGroup = comboBoxAttrGroup->findText(modelData(
                                                     DBATTRXML::ATTR,
                                                     DBATTRXML::GROUP,
                                                     index).toString());
    comboBoxAttrGroup->setCurrentIndex(indexGroup);

    int indexRef = comboBoxLinkAttr->findText(modelData(
                                                  DBATTRXML::ATTR,
                                                  DBATTRXML::REFCLASS,
                                                  index).toString());
    comboBoxLinkAttr->setCurrentIndex(indexRef);
    changeType(comboBoxTypeAttr->currentText());
    emit currentIndexChanged(index);
}
