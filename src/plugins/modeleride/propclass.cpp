#include "propclass.h"
#include "dlgeditattr.h"
#include "dbxmlstruct.h"
#include <treexmlmodel/xmldelegate.h>

PropClass::PropClass(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_mapper = new QDataWidgetMapper();
    m_mapperAttr = new QDataWidgetMapper();
    m_attrModel = new TableXMLProxyModel();

    m_typeAttrModel = new QStringListModel();
    m_typeAttrModel->setStringList(DBXMLATTRTYPE);

    m_typeClassModel = new QStringListModel();
    m_typeClassModel->setStringList(DBXMLCLASSTYPE);

    connect(tableViewAttr,SIGNAL(clicked(QModelIndex)),
            this,SLOT(setCurrentModelIndexAttr(QModelIndex)));
    connect(toolButtonAddAttr,SIGNAL(clicked()),this,SLOT(addPropAttr()));
    connect(toolButtonAddClass,SIGNAL(clicked()),this,SLOT(addPropClass()));
    connect(toolButtonDeleteAttr,SIGNAL(clicked()),this,SLOT(removePropAttr()));
    connect(toolButtonDelClass,SIGNAL(clicked()),this,SLOT(removePropClass()));
}

PropClass::~PropClass()
{
    /*delete m_mapperAttr;
    delete m_attrModel;
    delete m_mapper;
    delete m_model;*/
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

    m_attrModel->setSourceModel(m_model);

    tableViewAttr->setModel(m_attrModel);

    m_mapperAttr->setModel(m_attrModel);
    m_mapperAttr->addMapping(lineEditAttrName,0);

    comboBoxLinkAttr->setModel(m_model);
    comboBoxTypeAttr->setModel(m_typeAttrModel);


    m_mapperAttr->addMapping(lineEditAttrName,
                         m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::NAME));
    m_mapperAttr->addMapping(comboBoxTypeAttr,
                         m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::TYPE));
    m_mapperAttr->addMapping(spinBoxStrinLen,
                         m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::MAXSTRLEN));
    m_mapperAttr->addMapping(comboBoxLinkAttr,
                         m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::REFCLASS));
    m_mapperAttr->addMapping(lineEditDefaultValue,
                         m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::INITIALVAL));
    m_mapperAttr->addMapping(spinBoxDownValue,
                         m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::LOWVAL));
    m_mapperAttr->addMapping(spinBoxUpValue,
                         m_model->indexDisplayedAttr(DBATTRXML::ATTR,
                                                       DBATTRXML::UPVAL));
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
}

void PropClass::setCurrentModelIndex(QModelIndex index)
{
    m_attrModel->setFilterIndex(index);
    m_attrModel->setFilterRole(Qt::UserRole);
    m_attrModel->setFilterRegExp(DBATTRXML::ATTR);

    tableViewAttr->setRootIndex(m_attrModel->mapFromSource(index));
    tableViewAttr->setCurrentIndex(tableViewAttr->rootIndex().child(0,0));

    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);

    this->setCurrentModelIndexAttr(tableViewAttr->rootIndex().child(0,0));
}

void PropClass::setCurrentModelIndexAttr(QModelIndex index)
{
        m_mapperAttr->setRootIndex(index.parent());
        m_mapperAttr->setCurrentModelIndex(index);
}

TreeXMLModel *PropClass::model()
{
    return m_model;
}

void PropClass::addPropAttr(){
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    m_model->setInsTagName(DBATTRXML::ATTR);
    if (m_model->insertRow(0,srcIndex)){
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        this->setCurrentModelIndexAttr(m_attrModel->mapFromSource(srcCurrentIndex));

        /* NOTE: Возможно нужно инициализировать
        // Длина строки
        model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),2),0);
        // Начальное значение
        model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),5),"");
        // Нижнее значение
        model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),6),0);
        // Верхнее значение
        model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),7),0);
        // Массив
        model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),8),false);
        // Длина массива
        model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),9),0);
        // Возможен NULL
        model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),10),false);
        // Уникальный ключ
        model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),11),false);
        // Кандидат в ключ
        model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(),12),false);
        */
        //showPropAttr();
    }

}

void PropClass::removePropAttr(){
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    m_model->setInsTagName(DBATTRXML::ATTR);
    QModelIndex curIndex = m_attrModel->mapToSource(tableViewAttr->currentIndex());
    m_model->removeRow(curIndex.row(),srcIndex);
}

void PropClass::addPropClass(){
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    m_model->setInsTagName(DBCLASSXML::CLASS);
    m_model->insertRow(0,srcIndex);
    qDebug() << m_model->lastInsertRow();
    setCurrentModelIndex(m_model->lastInsertRow());
    tabWidgetProp->setCurrentIndex(0);
}

void PropClass::removePropClass(){
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    m_model->setInsTagName(DBCLASSXML::CLASS);
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
