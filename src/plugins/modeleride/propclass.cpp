#include "propclass.h"
#include "dbxmlstruct.h"
#include <treexmlmodel/xmldelegate.h>

PropClass::PropClass(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    m_mapperAttr = new QDataWidgetMapper();
    m_mapperAttr->setItemDelegate(new XmlDelegate(this));
    m_mapperAttr->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_attrModel = new TableXMLProxyModel();
    QStringList tags;
    tags << DBATTRXML::ATTR;
    m_attrModel->setAttributeTags(tags);

    m_typeAttrModel = new QStringListModel();
    m_typeAttrModel->setStringList(DBXMLATTRTYPE);

    m_typeClassModel = new QStringListModel();
    m_typeClassModel->setStringList(DBXMLCLASSTYPE);

    tableViewAttr->setItemDelegate(new XmlDelegate(this));
    tableViewAttr->setEditTriggers(QAbstractItemView::NoEditTriggers);
    lineEditClassParent->setReadOnly(true);

    connect(tableViewAttr,SIGNAL(clicked(QModelIndex)),
            this,SLOT(setCurrentAttr(QModelIndex)));
    connect(toolButtonAddAttr,SIGNAL(clicked()),this,SLOT(addAttr()));
    connect(toolButtonAddClass,SIGNAL(clicked()),this,SLOT(addClass()));
    connect(toolButtonDeleteAttr,SIGNAL(clicked()),this,SLOT(removeAttr()));
    connect(toolButtonDelClass,SIGNAL(clicked()),this,SLOT(removeClass()));

    //connect(checkBoxIsLength,SIGNAL(toggled(bool)),this,SLOT(setCheckArray(bool)));
    connect(comboBoxTypeAttr,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeType(QString)));
    connect(pushButtonAttrSave,SIGNAL(clicked()),this,SLOT(submitAttr()));
    connect(pushButtonPropSave,SIGNAL(clicked()),this,SLOT(submitClass()));
    connect(pushButtonAttrCancel,SIGNAL(clicked()),this,SLOT(revertAttr()));
    connect(pushButtonPropCancel,SIGNAL(clicked()),this,SLOT(revertClass()));
    connect(toolButtonEditAttr,SIGNAL(clicked()),this,SLOT(editAttr()));
    connect(toolButtonEditClass,SIGNAL(clicked()),this,SLOT(editClass()));
    connect(checkBoxInInherited,SIGNAL(clicked(bool)),this,SLOT(setShowParentAttr(bool)));
}

PropClass::~PropClass()
{
    if (lineEditAttrName->text().isEmpty())
        m_attrModel->removeRow(m_mapperAttr->currentIndex(),
                               m_mapperAttr->rootIndex());
    delete m_mapperAttr;
    delete m_attrModel;
    if (lineEditClassName->text().isEmpty()){
        QModelIndex parentIndex = m_mapper->rootIndex();
        if (parentIndex.isValid()){
            m_model->removeRow(m_mapper->currentIndex(),
                               m_mapper->rootIndex());
        }
    }
    delete m_mapper;
    delete m_typeAttrModel;
    delete m_typeClassModel;
}

void PropClass::setModel(TreeXMLModel *model)
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

void PropClass::setCurrentClass(QModelIndex index)
{
    if (m_mapper->rootIndex() == index.parent() &&
            index.row() == m_mapper->currentIndex())
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

    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);
    editClass(false);
    m_attrModel->setSourceModel(m_model);
    tableViewAttr->setRootIndex(m_attrModel->mapFromSource(index));
    tableViewAttr->setCurrentIndex(tableViewAttr->rootIndex().child(0,0));
    int indexType = comboBoxClassType->findText(modelData(DBCLASSXML::CLASS,
                                                          DBCLASSXML::TYPE,
                                                          index).toString());
    comboBoxClassType->setCurrentIndex(indexType);

    this->setCurrentAttr(tableViewAttr->rootIndex().child(0,0));
    setTabName(index);
}

void PropClass::setTabName(const QModelIndex &index){
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString className = modelData(DBATTRXML::ATTR, DBATTRXML::NAME,index).toString();

    this->setObjectName("PropClass::" + className);
    subWindow->setWindowTitle(tr("Класс: ")+className);
}

QVariant PropClass::modelData(QString typeName, QString attr, const QModelIndex& index)
{
    return index.sibling(index.row(), m_model->indexDisplayedAttr(
                      typeName,attr)).data();
}

void PropClass::submitAttr()
{
    editAttr(false);
    m_mapperAttr->submit();
}

void PropClass::editAttr(bool flag)
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

void PropClass::editClass(bool flag)
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

void PropClass::removeEmptyClass(){
    if (lineEditClassName->text().isEmpty()){
        QModelIndex parentIndex = m_mapper->rootIndex();
        if (parentIndex.isValid()){
            m_model->removeRow(m_mapper->currentIndex(),
                               m_mapper->rootIndex());

            setCurrentClass(parentIndex);
        }else {
            removeClass();
            return;
        }
    }
}

void PropClass::submitClass()
{
    m_mapper->submit();
    removeEmptyClass();
    setTabName(m_mapper->rootIndex().child(m_mapper->currentIndex(),0));
    editClass(false);
}

void PropClass::revertAttr()
{
    m_mapperAttr->revert();
    editAttr(false);
}

void PropClass::revertClass()
{
    m_mapper->revert();
    removeEmptyClass();
    editClass(false);
}

void PropClass::setShowParentAttr(bool flag)
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

void PropClass::setCurrentAttr(QModelIndex index)
{
    if (!index.isValid())
        return;

    if (m_mapperAttr->rootIndex() == index.parent() &&
                index.row() == m_mapperAttr->currentIndex())
            return;

        editAttr(false);
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
}

TreeXMLModel *PropClass::model()
{
    return m_model;
}

void PropClass::addAttr(){
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    m_model->setInsTagName(DBATTRXML::ATTR);
    if (m_model->insertRow(0,srcIndex)){
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        this->setCurrentAttr(m_attrModel->mapFromSource(srcCurrentIndex));
        comboBoxTypeAttr->setCurrentIndex(5);
        editAttr(true);
    }
}

void PropClass::removeAttr(){
    editAttr(false);
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    QModelIndex curIndex = m_attrModel->mapToSource(tableViewAttr->currentIndex());
    if (srcIndex.isValid() && curIndex.isValid()){
        m_model->removeRow(curIndex.row(),srcIndex);
        this->setCurrentAttr(tableViewAttr->currentIndex());
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить атрибут, поскольку нет выбраных атрибутов."));
}

void PropClass::addClass(){
    editAttr(false);
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    m_model->setInsTagName(DBCLASSXML::CLASS);
    m_model->insertRow(0,srcIndex);
    QModelIndex srcCurrentIndex = m_model->lastInsertRow();
    setCurrentClass(srcCurrentIndex);

    editClass(true);
    tabWidgetProp->setCurrentIndex(0);
}

void PropClass::removeClass(){
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    subWindow->close();
    m_mapper->revert();
    m_mapperAttr->revert();
    if (!modelData(DBCLASSXML::CLASS,DBCLASSXML::NAME,srcIndex).toString().isEmpty())
            m_model->removeRow(srcIndex.row(),srcIndex.parent());
}

void PropClass::changeType(QString s)
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
