#include "attrwidget.h"
#include "dbxmlstruct.h"
#include "xmldelegate.h"
#include <QStringListModel>
#include <QMessageBox>
#include <QToolTip>
#include "treefilterproxymodel.h"
#include "regexpvalidator.h"

AttrWidget::AttrWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9_]{0,26}|^[A-Za-z]{0}"));
    lineEditAttrName->setValidator(validator);
    connect(validator,SIGNAL(stateChanged(QValidator::State)),
            this,SLOT(validateAttrName(QValidator::State)));

    m_mapperAttr = new QDataWidgetMapper();
    m_mapperAttr->setItemDelegate(new XmlDelegate(this));
    m_mapperAttr->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_attrModel = new TableXMLProxyModel();
    QStringList tags;
    tags << DBATTRXML::ATTR;
    m_attrModel->setAttributeTags(tags);

    m_typeAttrModel = new QStringListModel();
    const QStringList attrTypeList = (QStringList()
                                       << DBTYPEXML::BOOLEAN
                                       << DBTYPEXML::BINARY
                                       << DBTYPEXML::CHAR
                                       << DBTYPEXML::DATE
                                       << DBTYPEXML::DECIMAL
                                       << DBTYPEXML::DIMENSION
                                       << DBTYPEXML::DOUBLE
                                       << DBTYPEXML::INTEGER
                                       << DBTYPEXML::STRING
                                       << DBTYPEXML::REFERENCE
                                       << DBTYPEXML::RANGE
                                       << DBTYPEXML::TIME
                                       << DBTYPEXML::TIMESHTAMP
                                       );
    m_typeAttrModel->setStringList(attrTypeList);

    tableViewAttr->setItemDelegate(new XmlDelegate(this));
    comboBoxLov->setItemDelegate(new XmlDelegate(this));
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

    connect(toolButtonUnitAttrClean, SIGNAL(clicked()), this, SLOT(cleanUnit()));
    connect(toolButtonLovClean, SIGNAL(clicked()), this, SLOT(cleanLov()));

    connect(toolButtonUpAttr,SIGNAL(clicked()),this,SLOT(up()));
    connect(toolButtonDownAttr,SIGNAL(clicked()),this,SLOT(down()));
}

AttrWidget::~AttrWidget()
{
    delete lineEditAttrName->validator();

    delete m_typeAttrModel;
    delete m_mapperAttr;
    delete m_attrModel;
}

void AttrWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    m_attrModel->setSourceModel(m_model);
    m_attrModel->setHeaderData(0,  Qt::Horizontal, tr("Имя атрибута"));
    m_attrModel->setHeaderData(1,  Qt::Horizontal, tr("Псевдоним"));
    m_attrModel->setHeaderData(2,  Qt::Horizontal, tr("Тип"));
    m_attrModel->setHeaderData(3,  Qt::Horizontal, tr("Длина строки"));
    m_attrModel->setHeaderData(4,  Qt::Horizontal, tr("Ссылочный класс"));
    m_attrModel->setHeaderData(5,  Qt::Horizontal, tr("Класс"));
    m_attrModel->setHeaderData(6,  Qt::Horizontal, tr("ЕИ"));
    m_attrModel->setHeaderData(7,  Qt::Horizontal, tr("По умолчанию"));
    m_attrModel->setHeaderData(8,  Qt::Horizontal, tr("Нижняя граница"));
    m_attrModel->setHeaderData(9,  Qt::Horizontal, tr("Верхняя гранница"));
    m_attrModel->setHeaderData(10, Qt::Horizontal, tr("Список значений"));
    m_attrModel->setHeaderData(11, Qt::Horizontal, tr("Группа"));
    m_attrModel->setHeaderData(12, Qt::Horizontal, tr("Нулевые значения"));
    m_attrModel->setHeaderData(13, Qt::Horizontal, tr("Уникальный"));
    m_attrModel->setHeaderData(14, Qt::Horizontal, tr("Кандидат в ключ"));
    m_attrModel->setHeaderData(15, Qt::Horizontal, tr("Индетификатор"));

    m_attrModel->setDynamicSortFilter(true);

    tableViewAttr->setModel(m_attrModel);
    tableViewAttr->setColumnHidden(15,true);

    m_mapperAttr->setModel(m_attrModel);

    TableXMLProxyModel* lovFilterModel = new TableXMLProxyModel(this);
    QStringList tags;
    tags << DBLOVLISTXML::LOVLIST;
    lovFilterModel->setAttributeTags(tags);
    lovFilterModel->setSourceModel(m_model);
    lovFilterModel->setFilterIndex(m_model->index(0,0));
    lovFilterModel->setFilterRole(Qt::EditRole);
    lovFilterModel->setFilterKeyColumn(m_model->columnDisplayedAttr(
                                           DBLOVXML::LOV,DBLOVXML::TYPE));
    lovFilterModel->setDynamicSortFilter(true);
    lovFilterModel->sort(0);
    comboBoxLov->setModel(lovFilterModel);
    comboBoxLov->setRootModelIndex(lovFilterModel->index(0,0).child(0,0));
    lovFilterModel->setFilterIndex(lovFilterModel->mapToSource(lovFilterModel->index(0,0).child(0,0)));
    tags << DBLOVXML::LOV;
    lovFilterModel->setAttributeTags(tags);

    QSortFilterProxyModel* classFilterModel = new QSortFilterProxyModel(this);
    classFilterModel->setFilterKeyColumn(0);
    classFilterModel->setFilterRole(TreeXmlModel::TagRole);
    classFilterModel->setFilterRegExp(DBCLASSXML::CLASS + "|" +
                                      DBMODELXML::MODEL + "|" +
                                      DBCLASSLISTXML::CLASSLIST);
    classFilterModel->setSourceModel(m_model);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);

    comboBoxLinkAttr->setModel(classFilterModel);
    comboBoxLinkAttr->setRootModelIndex(classFilterModel->index(0,0).child(0,0));
    comboBoxLinkAttr->setIndexColumn(m_model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                                  DBCLASSXML::ID));

    QSortFilterProxyModel* unitFilterModel = new QSortFilterProxyModel(this);
    unitFilterModel->setFilterKeyColumn(0);
    unitFilterModel->setFilterRole(TreeXmlModel::TagRole);
    unitFilterModel->setFilterRegExp(DBUNITXML::UNIT + "|" +
                                     DBMODELXML::MODEL + "|" +
                                     DBENTITYLISTXML::ENTITYLIST + "|" +
                                     DBENTITYGROUPXML::ENTITYGROUP + "|" +
                                     DBENTITYXML::ENTITY);
    unitFilterModel->setSourceModel(m_model);
    unitFilterModel->setDynamicSortFilter(true);
    unitFilterModel->sort(0);

    comboBoxUnitAttr->setModel(unitFilterModel);
    comboBoxUnitAttr->setRootModelIndex(unitFilterModel->index(0,0).child(0,0));
    comboBoxUnitAttr->setIndexColumn(
                m_model->columnDisplayedAttr(DBUNITXML::UNIT,
                                             DBUNITXML::ID));

    comboBoxTypeAttr->setModel(m_typeAttrModel);

    m_mapperAttr->addMapping(lineEditAttrName,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::NAME));
    m_mapperAttr->addMapping(comboBoxTypeAttr,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::TYPE));
    m_mapperAttr->addMapping(spinBoxStringLen,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::MAXSTRLEN));
    m_mapperAttr->addMapping(comboBoxLinkAttr,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::REFCLASS));
    m_mapperAttr->addMapping(lineEditDefaultValue,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::INITIALVAL));
    m_mapperAttr->addMapping(lineEditLowerBound,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::LOWERBOUND));
    m_mapperAttr->addMapping(lineEditUpperBound,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::UPPERBOUND));
    m_mapperAttr->addMapping(checkBoxIsNull,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::ISNULLALLOWED));
    m_mapperAttr->addMapping(checkBoxIsUnique,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::ISUNIQUE));
    m_mapperAttr->addMapping(checkBoxIsCandidateKey,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::ISCANDIDATEKEY));
    m_mapperAttr->addMapping(lineEditAttrAlias,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::ALIAS));
    m_mapperAttr->addMapping(comboBoxAttrGroup,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::GROUP));
    m_mapperAttr->addMapping(comboBoxUnitAttr,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::REFUNIT));

    m_mapperAttr->addMapping(comboBoxLov,
                             m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                         DBATTRXML::REFLOV));

}

void AttrWidget::setRootIndex(const QModelIndex &index)
{
    QModelIndex rootIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    if (rootIndex == index)
        return;

    m_attrModel->setFilterIndex(index);
    for (int row=0;row<comboBoxAttrGroup->count();row++)
        comboBoxAttrGroup->removeItem(row);

    QModelIndex srcIndex = m_attrModel->mapFromSource(index);
    int groupColumn = m_model->columnDisplayedAttr(DBATTRXML::ATTR, DBATTRXML::GROUP);
    int row = 0;
    QModelIndex srcChildIndex = srcIndex.child(row, groupColumn);
    while (srcChildIndex.isValid())
    {
        QString insText = srcChildIndex.data().toString();
        if (comboBoxAttrGroup->findText(insText)==-1)
            comboBoxAttrGroup->addItem(insText);
        srcChildIndex = srcIndex.child(++row, groupColumn);
    }

    m_attrModel->setSourceModel(m_model);
    tableViewAttr->setRootIndex(m_attrModel->mapFromSource(index));
    tableViewAttr->setCurrentIndex(tableViewAttr->rootIndex().child(0,0));
    m_mapperAttr->setRootIndex(m_attrModel->mapFromSource(index));

    this->setCurrent(tableViewAttr->rootIndex().child(0,0));
}

void AttrWidget::validateAttrName(QValidator::State state) const
{
    if(state != QValidator::Acceptable)
        QToolTip::showText(lineEditAttrName->mapToGlobal(QPoint(0,5)),
                           tr("Имя атрибута должно содержать только латинские\n"
                              "символы и цифры длиной не более 27 символов"));
    else
        QToolTip::hideText();
}

void AttrWidget::cleanUnit()
{
    comboBoxUnitAttr->setDisplayText("");
    comboBoxUnitAttr->setCurrentIndex(-1);
}

void AttrWidget::cleanLov()
{
    comboBoxLov->setCurrentIndex(-1);
}

void AttrWidget::add()
{
    QModelIndex srcIndex = m_attrModel->mapToSource(tableViewAttr->rootIndex());
    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex,DBATTRXML::ATTR);
    if (srcCurrentIndex.isValid()){
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
    QModelIndex rootIndex = (tableViewAttr->rootIndex());
    QModelIndex srcIndex = m_mapperAttr->rootIndex().child(m_mapperAttr->currentIndex(),0);
    int nameColumn = m_model->columnDisplayedAttr(DBATTRXML::ATTR, DBATTRXML::NAME);

    int row = 0;
    QModelIndex childIndex = m_attrModel->index(row, nameColumn, rootIndex);
    while (childIndex.isValid())
    {
        if (lineEditAttrName->text().toUpper() == childIndex.data().toString().toUpper()
                && childIndex.sibling(row,0)!=srcIndex) {
            QMessageBox::warning(this,tr("Предуреждение"),
                                 tr("Атрибут с таким имененм уже существует"));
            return;
        }
        childIndex = m_attrModel->index(++row, nameColumn, rootIndex);
    }
    edit(false);
    m_mapperAttr->submit();

    // Занулить если выбраны неверные данные в comboBoxUnitAttr
    QModelIndex unitIndex = srcIndex.sibling(
                srcIndex.row(),
                m_model->columnDisplayedAttr(DBATTRXML::ATTR, DBATTRXML::REFUNIT));
    if (unitIndex.data().isNull()) {
        comboBoxUnitAttr->setCurrentIndex(-1);
    }

    emit dataChanged(srcIndex);
}

bool AttrWidget::isEdit()
{
    return groupBoxPropAttr->isEnabled();
}

void AttrWidget::edit(bool flag)
{
    if (isEdit()==flag && tableViewAttr->currentIndex().isValid())
        return;

    if (lineEditAttrName->text().isEmpty() && flag==false)
        m_attrModel->removeRow(m_mapperAttr->currentIndex(),
                               m_mapperAttr->rootIndex());

    if (!tableViewAttr->currentIndex().isValid()){
        flag = false;
        toolButtonDeleteAttr->setEnabled(false);
        toolButtonEditAttr->setEnabled(false);
        toolButtonAddAttr->setEnabled(true);
    } else {
        toolButtonEditAttr->setDisabled(flag);
        toolButtonDeleteAttr->setDisabled(flag);
        toolButtonAddAttr->setDisabled(flag);
    }


    groupBoxPropAttr->setEnabled(flag);
    groupBoxPropType->setEnabled(flag);
    pushButtonAttrSave->setEnabled(flag);
    pushButtonAttrCancel->setEnabled(flag);

}

void AttrWidget::up()
{
    QPersistentModelIndex index = tableViewAttr->currentIndex();
    QPersistentModelIndex srcIndex  = m_attrModel->mapToSource(index);
    QPersistentModelIndex srcParent = srcIndex.parent();
    int row = m_attrModel->mapToSource(index.sibling(index.row()-1,0)).row();
    if (m_model->moveIndex(srcIndex,srcParent,row)) {
        if (row >= 0)
            index = tableViewAttr->currentIndex().sibling(index.row()-2,0);
        else
            index = index.sibling(0,0);
        m_model->removeRow(srcIndex.row(),srcIndex.parent());
        setCurrent(index);
    }
}

void AttrWidget::down()
{
    QPersistentModelIndex index = tableViewAttr->currentIndex();
    QPersistentModelIndex srcIndex  = m_attrModel->mapToSource(index);
    QPersistentModelIndex srcParent = srcIndex.parent();
    QModelIndex indexNew = index.sibling(index.row()+2,0);
    int row = (indexNew.isValid()) ? m_attrModel->mapToSource(indexNew).row()
                                   : m_model->rowCount(srcParent,
                                                       m_model->tagsFilter(),
                                                       QStringList());
    if (m_model->moveIndex(srcIndex,srcParent,row)) {
        index = tableViewAttr->currentIndex().sibling(index.row()+2,0);
        m_model->removeRow(srcIndex.row(),srcIndex.parent());
        setCurrent(index);
    }
}

void AttrWidget::revert()
{
    m_mapperAttr->revert();
    this->setCurrent(tableViewAttr->currentIndex());
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
                                      DBCLASSXML::ID,
                                      index).toString();
        className.replace("{","\\{");
        className.replace("}","\\}");
        if (className.isEmpty()){
            m_attrModel->setFilterRegExp("\\S*");
        }else
            m_attrModel->setFilterRegExp(className);
    }
    m_attrModel->setFilterKeyColumn(m_model->columnDisplayedAttr(DBATTRXML::ATTR,
                                                                DBATTRXML::PARENT));
}

void AttrWidget::changeType(const QString &typeName)
{
    if (DBTYPEXML::STRING==typeName){
        // String
        spinBoxStringLen->setEnabled(true);
        comboBoxLinkAttr->setEnabled(false);
        comboBoxLinkAttr->setDisplayText("");
        comboBoxLinkAttr->setCurrentIndex(-1);
        comboBoxLov->setEnabled(true);
        lineEditDefaultValue->setEnabled(true);
        lineEditLowerBound->setEnabled(true);
        lineEditUpperBound->setEnabled(true);
        comboBoxUnitAttr->setEnabled(false);
        comboBoxUnitAttr->setDisplayText("");
        comboBoxUnitAttr->setCurrentIndex(-1);
    } else if(DBTYPEXML::REFERENCE==typeName){
        //Reference
        spinBoxStringLen->setEnabled(false);
        spinBoxStringLen->setValue(0);
        comboBoxLinkAttr->setEnabled(true);
        comboBoxLov->setEnabled(false);
        lineEditDefaultValue->setEnabled(false);
        lineEditDefaultValue->setText("");
        lineEditLowerBound->setEnabled(false);
        lineEditLowerBound->setText("");
        lineEditUpperBound->setEnabled(false);
        lineEditUpperBound->setText("");
        comboBoxUnitAttr->setEnabled(false);
        comboBoxUnitAttr->setDisplayText("");
        comboBoxUnitAttr->setCurrentIndex(-1);
    } else {
        spinBoxStringLen->setEnabled(false);
        spinBoxStringLen->setValue(0);
        comboBoxLinkAttr->setEnabled(false);
        comboBoxLinkAttr->setDisplayText("");
        comboBoxLinkAttr->setCurrentIndex(-1);
        comboBoxLov->setEnabled(true);
        lineEditDefaultValue->setEnabled(true);
        lineEditLowerBound->setEnabled(true);
        lineEditUpperBound->setEnabled(true);

        // Decimal, Dimension, Double, Integer, Rage
        if( DBTYPEXML::DECIMAL   == typeName
         || DBTYPEXML::DIMENSION == typeName
         || DBTYPEXML::DOUBLE    == typeName
         || DBTYPEXML::INTEGER   == typeName
         || DBTYPEXML::RANGE     == typeName )
        {
            comboBoxUnitAttr->setEnabled(true);
        } else {
            comboBoxUnitAttr->setEnabled(false);
            comboBoxUnitAttr->setDisplayText("");
            comboBoxUnitAttr->setCurrentIndex(-1);
        }
    }

    TableXMLProxyModel *lovFilterModel = qobject_cast<TableXMLProxyModel *>(comboBoxLov->model());
    lovFilterModel->setFilterRegExp(typeName);
    if (groupBoxPropType->isEnabled())
      comboBoxLov->setCurrentIndex(-1);
}

QVariant AttrWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}


void AttrWidget::setCurrent(const QModelIndex &index)
{
    edit(false);
    if (!index.isValid())
        return;

    tableViewAttr->setCurrentIndex(index);


    int indexType = comboBoxTypeAttr->findText(modelData(
                                                   DBATTRXML::ATTR,
                                                   DBATTRXML::TYPE,
                                                   index).toString());
    comboBoxTypeAttr->setCurrentIndex(indexType);

    m_mapperAttr->setCurrentModelIndex(index);

    comboBoxAttrGroup->setEditText(modelData(DBATTRXML::ATTR,DBATTRXML::GROUP,
                                                     index).toString());
    emit currentIndexChanged(index);
}
