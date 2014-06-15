#include "msrunitwidget.h"
#include "dbxmlstruct.h"
#include "xmldelegate.h"
#include <QStringListModel>
#include <QMessageBox>
#include "treefilterproxymodel.h"

MsrUnitWidget::MsrUnitWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_mapperUnit = new QDataWidgetMapper();
    m_mapperUnit->setItemDelegate(new XmlDelegate(this));
    m_mapperUnit->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_unitModel = new TableXMLProxyModel();
    QStringList tags;
    tags << DBMSRUNITXML::UNIT;
    m_unitModel->setAttributeTags(tags);

//    m_typeAttrModel = new QStringListModel();
//    m_typeAttrModel->setStringList(DBXMLATTRTYPE);

    tableViewUnit->setItemDelegate(new XmlDelegate(this));
    tableViewUnit->setEditTriggers(QAbstractItemView::NoEditTriggers);

//    connect(tableViewAttr,SIGNAL(clicked(QModelIndex)),
//            this,SLOT(setCurrent(QModelIndex)));
//    connect(toolButtonAddAttr,SIGNAL(clicked()),this,SLOT(add()));
//    connect(toolButtonDeleteAttr,SIGNAL(clicked()),this,SLOT(remove()));

//    connect(comboBoxTypeAttr,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeType(QString)));
//    connect(pushButtonAttrSave,SIGNAL(clicked()),this,SLOT(submit()));
//    connect(pushButtonAttrCancel,SIGNAL(clicked()),this,SLOT(revert()));
//    connect(toolButtonEditAttr,SIGNAL(clicked()),this,SLOT(edit()));
//    connect(checkBoxInInherited,SIGNAL(clicked(bool)),this,SLOT(showParentAttr(bool)));
}

MsrUnitWidget::~MsrUnitWidget()
{
    delete m_mapperUnit;
    delete m_unitModel;
}

void MsrUnitWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    m_unitModel->setSourceModel(m_model);
    m_unitModel->setHeaderData(0,  Qt::Horizontal, tr("Обозначение"));
    m_unitModel->setHeaderData(1,  Qt::Horizontal, tr("Условное обозначение"));
    m_unitModel->setHeaderData(2,  Qt::Horizontal, tr("Код ОКЕИ"));
    m_unitModel->setHeaderData(3,  Qt::Horizontal, tr("Коэффициент"));
    m_unitModel->setHeaderData(4,  Qt::Horizontal, tr("Разница"));
    m_unitModel->setHeaderData(5,  Qt::Horizontal, tr("Сущность ЕИ"));
    m_unitModel->setHeaderData(6,  Qt::Horizontal, tr("Индентификатор"));
    m_unitModel->setDynamicSortFilter(true);

    tableViewUnit->setSortingEnabled(true);
    tableViewUnit->setModel(m_unitModel);
    tableViewUnit->sortByColumn(0,Qt::AscendingOrder);
    tableViewUnit->setColumnHidden(5,true);
    tableViewUnit->setColumnHidden(6,true);
    tableViewUnit->setColumnHidden(7,true);
    tableViewUnit->setColumnHidden(8,true);
    tableViewUnit->setColumnHidden(9,true);
    tableViewUnit->setColumnHidden(10,true);
    tableViewUnit->setColumnHidden(11,true);
    tableViewUnit->setColumnHidden(12,true);
    tableViewUnit->setColumnHidden(13,true);

    m_mapperUnit->setModel(m_unitModel);

    QSortFilterProxyModel* classFilterModel = new QSortFilterProxyModel(this);
//    classFilterModel->setFilterKeyColumn(0);
//    classFilterModel->setFilterRole(Qt::UserRole);
//    classFilterModel->setFilterRegExp(DBCLASSXML::CLASS);
    classFilterModel->setSourceModel(m_model);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);

//    comboBoxLinkAttr->setModel(classFilterModel);
//    comboBoxLinkAttr->setIndexColumn(m_model->columnDisplayedAttr(DBCLASSXML::CLASS,
//                                                                 DBATTRXML::ID));

//    comboBoxTypeAttr->setModel(m_typeAttrModel);

//    m_mapperAttr->addMapping(lineEditAttrName,
//                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                         DBATTRXML::NAME));
//    m_mapperAttr->addMapping(comboBoxTypeAttr,
//                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                         DBATTRXML::TYPE));
//    m_mapperAttr->addMapping(spinBoxStringLen,
//                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                         DBATTRXML::MAXSTRLEN));
//    m_mapperAttr->addMapping(comboBoxLinkAttr,
//                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                         DBATTRXML::REFCLASS));
//    m_mapperAttr->addMapping(lineEditDefaultValue,
//                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                         DBATTRXML::INITIALVAL));
//    m_mapperAttr->addMapping(lineEditLowerBound,
//                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                         DBATTRXML::LOWERBOUND));
//    m_mapperAttr->addMapping(lineEditUpperBound,
//                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                         DBATTRXML::UPPERBOUND));
//    m_mapperAttr->addMapping(checkBoxIsNull,
//                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                         DBATTRXML::ISNULLALLOWED));
//    m_mapperAttr->addMapping(checkBoxIsUnique,
//                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                         DBATTRXML::ISUNIQUE));
//    m_mapperAttr->addMapping(checkBoxIsCandidateKey,
//                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                         DBATTRXML::ISCANDIDATEKEY));
//    m_mapperAttr->addMapping(lineEditAttrDesc,
//                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                         DBATTRXML::DESCRIPTION));
//    m_mapperAttr->addMapping(comboBoxAttrGroup,
//                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                         DBATTRXML::GROUP));
}

void MsrUnitWidget::add()
{
    QModelIndex srcIndex = m_unitModel->mapToSource(tableViewUnit->rootIndex());
    m_model->setInsTagName(DBMSRUNITXML::UNIT);
    if (m_model->insertRow(0,srcIndex)){
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        this->setCurrent(m_unitModel->mapFromSource(srcCurrentIndex));
//        comboBoxTypeAttr->setCurrentIndex(5);
        edit(true);
    }
}

void MsrUnitWidget::remove()
{
    edit(false);
    QModelIndex srcIndex = m_unitModel->mapToSource(tableViewUnit->rootIndex());
    QModelIndex curIndex = m_unitModel->mapToSource(tableViewUnit->currentIndex());
    if (srcIndex.isValid() && curIndex.isValid()){
        emit dataRemoved(srcIndex);
        m_model->removeRow(curIndex.row(),srcIndex);
        this->setCurrent(tableViewUnit->currentIndex());
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить атрибут, поскольку нет выбраных атрибутов."));
}

void MsrUnitWidget::setCurrent(const QModelIndex &index)
{
    edit(false);
    if (!index.isValid())
        return;

    tableViewUnit->setCurrentIndex(index);
    m_mapperUnit->setCurrentModelIndex(index);

//    int indexType = comboBoxTypeAttr->findText(modelData(
//                                                   DBATTRXML::ATTR,
//                                                   DBATTRXML::TYPE,
//                                                   index).toString());
//    comboBoxTypeAttr->setCurrentIndex(indexType);

//    comboBoxAttrGroup->setEditText(modelData(DBATTRXML::ATTR,DBATTRXML::GROUP,
//                                                     index).toString());

//    changeType(comboBoxTypeAttr->currentText());
    emit currentIndexChanged(index);
}

void MsrUnitWidget::submit()
{
    QModelIndex rootIndex = (tableViewUnit->rootIndex());
    QModelIndex srcIndex = m_mapperUnit->rootIndex().child(m_mapperUnit->currentIndex(),0);
    for (int row=0; row < m_unitModel->rowCount(rootIndex); row++){
        QModelIndex childIndex = m_unitModel->index(row,
                                                    m_model->columnDisplayedAttr(
                                                        DBMSRUNITXML::UNIT,
                                                        DBMSRUNITXML::DESIGNATION),
                                                    rootIndex);
//        if (lineEditAttrName->text() == childIndex.data()
//                && childIndex.sibling(row,0)!=srcIndex) {
//            QMessageBox::warning(this,tr("Предуреждение"),
//                                 tr("Атрибут с таким имененм уже существует"));
//            return;
//        }
    }
    edit(false);
    m_mapperUnit->submit();
    emit dataChanged(srcIndex);
}

void MsrUnitWidget::edit(bool flag)
{
//    if (groupBoxPropAttr->isEnabled()==flag && tableViewAttr->currentIndex().isValid())
//        return;

//    if (lineEditAttrName->text().isEmpty() && flag==false)
//        m_attrModel->removeRow(m_mapperAttr->currentIndex(),
//                               m_mapperAttr->rootIndex());

//    if (!tableViewAttr->currentIndex().isValid()){
//        flag = false;
//        toolButtonDeleteAttr->setEnabled(false);
//        toolButtonEditAttr->setEnabled(false);
//        toolButtonAddAttr->setEnabled(true);
//    } else {
//        toolButtonEditAttr->setDisabled(flag);
//        toolButtonDeleteAttr->setEnabled(true);
//        toolButtonAddAttr->setDisabled(flag);
//    }


//    groupBoxPropAttr->setEnabled(flag);
//    groupBoxPropType->setEnabled(flag);
//    pushButtonAttrSave->setEnabled(flag);
//    pushButtonAttrCancel->setEnabled(flag);
}

void MsrUnitWidget::revert()
{
    m_mapperUnit->revert();
    this->setCurrent(tableViewUnit->currentIndex());
    edit(false);
}

void MsrUnitWidget::setRootIndex(const QModelIndex &index)
{
    QModelIndex rootIndex = m_unitModel->mapToSource(tableViewUnit->rootIndex());
    if (rootIndex == index)
        return;

    m_unitModel->setFilterIndex(index);
//    for (int row=0;row<comboBoxAttrGroup->count();row++)
//        comboBoxAttrGroup->removeItem(row);

//    for (int row=0;row<m_attrModel->rowCount(m_attrModel->mapFromSource(index));row++){
//        QString insText = m_attrModel->mapFromSource(index).child(
//                    row,m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                    DBATTRXML::GROUP)).data().toString();
//        if (comboBoxAttrGroup->findText(insText)==-1)
//            comboBoxAttrGroup->addItem(insText);
//    }

    m_unitModel->setSourceModel(m_model);
    tableViewUnit->setRootIndex(m_unitModel->mapFromSource(index));
    tableViewUnit->setCurrentIndex(tableViewUnit->rootIndex().child(0,0));
    m_mapperUnit->setRootIndex(m_unitModel->mapFromSource(index));

    this->setCurrent(tableViewUnit->rootIndex().child(0,0));
}

QVariant MsrUnitWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}
