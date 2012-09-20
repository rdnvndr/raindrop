#include "propclass.h"
#include "dlgeditattr.h"
#include "dbxmlstruct.h"
#include <treexmlmodel/xmldelegate.h>

PropClass::PropClass(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapperAttr = new QDataWidgetMapper();
    m_mapperAttr->setItemDelegate(new XmlDelegate(this));

    m_attrModel = new TableXMLProxyModel();

    m_typeAttrModel = new QStringListModel();
    m_typeAttrModel->setStringList(DBXMLATTRTYPE);

    m_typeClassModel = new QStringListModel();
    m_typeClassModel->setStringList(DBXMLCLASSTYPE);

    tableViewAttr->setItemDelegate(new XmlDelegate(this));

    connect(tableViewAttr,SIGNAL(clicked(QModelIndex)),
            this,SLOT(setCurrentAttr(QModelIndex)));
    connect(toolButtonAddAttr,SIGNAL(clicked()),this,SLOT(addAttr()));
    connect(toolButtonAddClass,SIGNAL(clicked()),this,SLOT(addClass()));
    connect(toolButtonDeleteAttr,SIGNAL(clicked()),this,SLOT(removeAttr()));
    connect(toolButtonDelClass,SIGNAL(clicked()),this,SLOT(removeClass()));

    //connect(checkBoxIsLength,SIGNAL(toggled(bool)),this,SLOT(setCheckArray(bool)));
    connect(comboBoxTypeAttr,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeType(QString)));
}

PropClass::~PropClass()
{
    delete m_mapperAttr;
    delete m_attrModel;
    delete m_mapper;
    delete m_typeAttrModel;
    delete m_typeClassModel;
}

void PropClass::setModel(TreeXMLModel *model)
{
    m_model = model;

    m_mapper->setModel(m_model);
    comboBoxClassParent->setModel(model);

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

    m_mapper->addMapping(comboBoxClassParent,
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
    m_attrModel->setFilterIndex(index);
    m_attrModel->setFilterRole(Qt::UserRole);
    m_attrModel->setFilterRegExp(DBATTRXML::ATTR);

    for (int row=0;row<comboBoxAttrGroup->count();row++)
        comboBoxAttrGroup->removeItem(row);

    for (int row=0;row<m_attrModel->rowCount(m_attrModel->mapFromSource(index));row++){
        QString insText = m_attrModel->mapFromSource(index).child(
                    row,m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                    DBATTRXML::GROUP)).data().toString();
        if (comboBoxAttrGroup->findText(insText)==-1)
            comboBoxAttrGroup->addItem(insText);
    }

    tableViewAttr->setRootIndex(m_attrModel->mapFromSource(index));
    tableViewAttr->setCurrentIndex(tableViewAttr->rootIndex().child(0,0));

    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);

    int indexType = comboBoxClassType->findText(index.sibling(index.row(),
                                             m_model->indexDisplayedAttr(
                                                 DBCLASSXML::CLASS,
                                                 DBCLASSXML::TYPE)
                                             ).data().toString());
    comboBoxClassType->setCurrentIndex(indexType);

    int indexParent = comboBoxClassParent->findText(index.sibling(index.row(),
                                             m_model->indexDisplayedAttr(
                                                 DBCLASSXML::CLASS,
                                                 DBCLASSXML::PARENT)
                                             ).data().toString());
    comboBoxClassParent->setCurrentIndex(indexParent);

    this->setCurrentAttr(tableViewAttr->rootIndex().child(0,0));
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString className = index.sibling(index.row(),
                                      m_model->indexDisplayedAttr(
                                          DBATTRXML::ATTR,
                                          DBATTRXML::NAME)
                                      ).data().toString();
    this->setObjectName("PropClass::" + className);
    subWindow->setWindowTitle(tr("Класс: ")+className);
}

void PropClass::setCurrentAttr(QModelIndex index)
{
        m_mapperAttr->setRootIndex(index.parent());
        m_mapperAttr->setCurrentModelIndex(index);

        int indexType = comboBoxTypeAttr->findText(index.sibling(index.row(),
                                                 m_model->indexDisplayedAttr(
                                                     DBATTRXML::ATTR,
                                                     DBATTRXML::TYPE)
                                                 ).data().toString());
        comboBoxTypeAttr->setCurrentIndex(indexType);

        int indexGroup = comboBoxAttrGroup->findText(index.sibling(index.row(),
                                                 m_model->indexDisplayedAttr(
                                                     DBATTRXML::ATTR,
                                                     DBATTRXML::GROUP)
                                                 ).data().toString());
        comboBoxAttrGroup->setCurrentIndex(indexGroup);

        int indexRef = comboBoxLinkAttr->findText(index.sibling(index.row(),
                                                 m_model->indexDisplayedAttr(
                                                     DBATTRXML::ATTR,
                                                     DBATTRXML::REFCLASS)
                                                 ).data().toString());
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

        m_model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                           DBATTRXML::NAME)),
                                                 "");
        m_model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                           DBATTRXML::TYPE)),
                                                 DBXMLATTRTYPE.at(5));
        m_model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                           DBATTRXML::MAXSTRLEN)),
                                                 0);
        m_model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                           DBATTRXML::REFCLASS)),
                                                 "");
        m_model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                           DBATTRXML::INITIALVAL)),
                                                 "");
        m_model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                           DBATTRXML::ISNULLALLOWED)),
                                                 true);
        m_model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                           DBATTRXML::ISUNIQUE)),
                                                 false);
        m_model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                           DBATTRXML::ISCANDIDATEKEY)),
                                                 false);
        m_model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                           DBATTRXML::DESCRIPTION)),
                                                 "");
        m_model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),
                             m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                           DBATTRXML::GROUP)),
                                                 "");
        this->setCurrentAttr(m_attrModel->mapFromSource(srcCurrentIndex));
    }
}

void PropClass::removeAttr(){
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    m_model->setInsTagName(DBATTRXML::ATTR);
    QModelIndex curIndex = m_attrModel->mapToSource(tableViewAttr->currentIndex());
    m_model->removeRow(curIndex.row(),srcIndex);

    this->setCurrentAttr(tableViewAttr->currentIndex());
}

void PropClass::addClass(){
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    m_model->setInsTagName(DBCLASSXML::CLASS);
    m_model->insertRow(0,srcIndex);
    setCurrentClass(m_model->lastInsertRow());
    tabWidgetProp->setCurrentIndex(0);
}

void PropClass::removeClass(){
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    m_model->setInsTagName(DBCLASSXML::CLASS);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    subWindow->close();

    m_model->removeRow(srcIndex.row(),srcIndex.parent());
}

void PropClass::showPropAttr()
{
    DlgEditAttr* dlgEditAttr = new DlgEditAttr();
    dlgEditAttr->setModel(m_attrModel);
    dlgEditAttr->setCurrentModelIndex(tableViewAttr->currentIndex());
    if (dlgEditAttr->exec() == QDialog::Accepted){

    }
    delete dlgEditAttr;
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
