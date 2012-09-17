#include "propclass.h"
#include "dlgeditattr.h"
#include "dbxmlstruct.h"
#include <treexmlmodel/xmldelegate.h>

PropClass::PropClass(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_mapper = new QDataWidgetMapper();
    m_attrModel = new TableXMLProxyModel();

    /*connect(pushButtonAdd,SIGNAL(clicked()),this,SLOT(addPropAttr()));
    connect(pushButtonEdit,SIGNAL(clicked()),this,SLOT(showPropAttr()));
    connect(pushButtonDel,SIGNAL(clicked()),this,SLOT(removePropAttr()));
    connect(pushButtonApply,SIGNAL(clicked()),m_mapper,SLOT(submit()));*/

}

void PropClass::setModel(TreeXMLModel *model)
{
    m_model = model;

    m_mapper->setModel(m_model);

    m_mapper->addMapping(lineEditClassName,
                         model->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBATTRXML::NAME));
    m_mapper->addMapping(checkBoxAbsClass,
                         model->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::ISABSTARCT));
    m_mapper->addMapping(checkBoxActiveClass,
                         model->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::ISACTIVE));
    m_mapper->addMapping(lineEditClassDesc,
                         model->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::DESCRIPTION));
    m_mapper->addMapping(comboBoxClassParent,
                         model->indexDisplayedAttr(DBCLASSXML::CLASS,
                                                   DBCLASSXML::PARENT));

    m_attrModel->setSourceModel(m_model);
    tableViewAttr->setModel(m_attrModel);
}

void PropClass::setCurrentModelIndex(QModelIndex index)
{
    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);

    m_attrModel->setFilterIndex(index);
    m_attrModel->setFilterRole(Qt::UserRole);
    m_attrModel->setFilterRegExp(DBATTRXML::ATTR);

    tableViewAttr->setRootIndex(m_attrModel->mapFromSource(index));
    tableViewAttr->setCurrentIndex(tableViewAttr->rootIndex().child(0,0));
    //tableViewAttr->setItemDelegate(new XmlDelegate(this));
}

TreeXMLModel *PropClass::model()
{
    return m_model;
}

void PropClass::addPropAttr(){
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    m_model->setInsTagName(DBATTRXML::ATTR);
    if (m_model->insertRow(0,srcIndex)){
        TreeXMLModel* model = qobject_cast<TreeXMLModel*>(m_attrModel->sourceModel());
        QModelIndex srcCurrentIndex = model->lastInsertRow();
        tableViewAttr->setCurrentIndex(m_attrModel->mapFromSource(srcCurrentIndex));
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
        showPropAttr();
    }

}

void PropClass::removePropAttr(){
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    m_model->setInsTagName(DBATTRXML::ATTR);
    QModelIndex curIndex = m_attrModel->mapToSource(tableViewAttr->currentIndex());
    m_model->removeRow(curIndex.row(),srcIndex);
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
