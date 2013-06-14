#include "filterpropwidget.h"
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QMenu>
#include "xmldelegate.h"
#include "dbxmlstruct.h"

FilterPropWidget::FilterPropWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    menuAddCondition = new QMenu(tr("Добавить"), this);
    actionAddBlock = menuAddCondition->addAction(tr("Блок"));
    actionAddCondition = menuAddCondition->addAction(tr("Выражение"));
    actionAddSubBlock = menuAddCondition->addAction(tr("Подблок"));
    actionAddSubCondition = menuAddCondition->addAction(tr("Подвыражение"));
    toolButtonCondAdd->setMenu(menuAddCondition);

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_conditionModel = new ConditionProxyModel();

    lineEditSrcClass->setReadOnly(true);

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDel,SIGNAL(clicked()),this,SLOT(remove()));
    connect(pushButtonPropSave,SIGNAL(clicked()),this,SLOT(submit()));
    connect(pushButtonPropCancel,SIGNAL(clicked()),this,SLOT(revert()));
    connect(toolButtonEdit,SIGNAL(clicked()),this,SLOT(edit()));

    connect(actionAddCondition,SIGNAL(triggered()),this,SLOT(addCondition()));
    connect(actionAddBlock,SIGNAL(triggered()),this,SLOT(addBlock()));
    connect(actionAddSubCondition,SIGNAL(triggered()),this,SLOT(addSubCondition()));
    connect(actionAddSubBlock,SIGNAL(triggered()),this,SLOT(addSubBlock()));

    connect(toolButtonCondAdd,SIGNAL(clicked()),toolButtonCondAdd,SLOT(showMenu()));
    connect(toolButtonCondDel,SIGNAL(clicked()),this,SLOT(removeCondition()));

    m_oldIndex = -1;
}

FilterPropWidget::~FilterPropWidget()
{
    delete m_conditionModel;
    delete m_mapper;
    delete actionAddBlock;
    delete actionAddCondition;
    delete actionAddSubBlock;
    delete actionAddSubCondition;
    delete menuAddCondition;
}

void FilterPropWidget::setModel(TreeXMLModel *model)
{
    m_model = model;
    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));

    m_mapper->setModel(m_model);

    QSortFilterProxyModel* classFilterModel = new QSortFilterProxyModel(this);
    classFilterModel->setFilterKeyColumn(0);
    classFilterModel->setFilterRole(Qt::UserRole);
    classFilterModel->setFilterRegExp(DBCLASSXML::CLASS);
    classFilterModel->setSourceModel(m_model);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);
    comboBoxDestClass->setModel(classFilterModel);
    comboBoxDestClass->setIndexColumn(m_model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                                  DBATTRXML::ID));

    m_mapper->addMapping(lineEditName,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::NAME));
    m_mapper->addMapping(lineEditDesc,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::DESCRIPTION));

    m_mapper->addMapping(lineEditDirectDesc,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::DIRECTDESCRIPTION));

    m_mapper->addMapping(lineEditInverseDesc,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::INVERSEDESCRIPTION));
    m_mapper->addMapping(lineEditSrcClass,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::PARENT));

    m_mapper->addMapping(comboBoxDestClass,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::CLASS));

    m_conditionModel->setSourceModel(model);
    m_conditionModel->setHeaderData(0,Qt::Horizontal,tr("Атрибут"));
    m_conditionModel->setHeaderData(1,Qt::Horizontal,tr("Оператор"));
    m_conditionModel->setHeaderData(2,Qt::Horizontal,tr("Значение"));
    m_conditionModel->setHeaderData(3,Qt::Horizontal,tr("Коннектор"));
    treeViewCondition->setModel(m_conditionModel);
}

void FilterPropWidget::add()
{
    m_oldIndex = m_mapper->currentIndex();
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),
                                          0,m_mapper->rootIndex()).parent();

    m_model->setInsTagName(DBFILTERXML::FILTER);
    if (m_model->insertRow(0,srcIndex)){
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

void FilterPropWidget::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(), 0,
                                          m_mapper->rootIndex());
    m_mapper->revert();
    setCurrent(srcIndex.parent());

    emit dataRemoved(srcIndex);
    m_model->removeRow(srcIndex.row(),srcIndex.parent());
}

void FilterPropWidget::addCondition()
{
    QModelIndex parent = (treeViewCondition->currentIndex().isValid())
            ? treeViewCondition->currentIndex().parent()
            : treeViewCondition->rootIndex();

    m_conditionModel->insertRow(0,parent);
    QModelIndex index = m_conditionModel->lastInsertRow();
    m_conditionModel->setData(index, DBCONDITIONXML::COND, Qt::UserRole);
    m_conditionModel->setData(index.sibling(index.row(),3),tr("И"));
}

void FilterPropWidget::addSubCondition()
{
    QModelIndex parent = treeViewCondition->rootIndex();
    if (treeViewCondition->currentIndex().isValid())
        if (treeViewCondition->currentIndex().parent().isValid())
            parent = treeViewCondition->currentIndex().parent();

    m_conditionModel->insertRow(0,parent);
    QModelIndex index = m_conditionModel->lastInsertRow();
    m_conditionModel->setData(index, DBCONDITIONXML::COND, Qt::UserRole);
    m_conditionModel->setData(index.sibling(index.row(),3),tr("И"));
}

void FilterPropWidget::removeCondition()
{
    QModelIndex index = treeViewCondition->currentIndex();
    if (index.isValid())
        m_conditionModel->removeRow(index.row(),index.parent());
    else
        QMessageBox::warning(this,tr("Предуреждение"),
                             tr("Удалить невозможно. Ничего не выбрано."));
}

void FilterPropWidget::addBlock()
{
    QModelIndex parent = (treeViewCondition->currentIndex().isValid())
            ? treeViewCondition->currentIndex().parent()
            : treeViewCondition->rootIndex();
    m_conditionModel->insertRow(0,parent);
    QModelIndex index = m_conditionModel->lastInsertRow();
    m_conditionModel->setData(index, DBFILTERBLOCKXML::BLOCK, Qt::UserRole);
    m_conditionModel->setData(index.sibling(index.row(),3),tr("И"));
}

void FilterPropWidget::addSubBlock()
{
    QModelIndex parent = treeViewCondition->rootIndex();
    if (treeViewCondition->currentIndex().isValid())
        if (treeViewCondition->currentIndex().parent().isValid())
            parent = treeViewCondition->currentIndex().parent();

    m_conditionModel->insertRow(0,parent);
    QModelIndex index = m_conditionModel->lastInsertRow();
    m_conditionModel->setData(index, DBFILTERBLOCKXML::BLOCK, Qt::UserRole);
    m_conditionModel->setData(index.sibling(index.row(),3),tr("И"));
}

void FilterPropWidget::setCurrent(const QModelIndex &index)
{
    if (m_mapper->rootIndex() == index.parent() &&
            index.row() == m_mapper->currentIndex())
        return;

    m_mapper->setRootIndex(index.parent());
    m_mapper->setCurrentModelIndex(index);
    edit(false);

    treeViewCondition->setRootIndex(m_conditionModel->mapFromSource(index));

    emit currentIndexChanged(index);
}

void FilterPropWidget::edit(bool flag)
{
    if (groupBoxProp->isEnabled()==flag)
        return;

    if (lineEditName->text().isEmpty()){
        toolButtonAdd->setDisabled(true);
        flag = true;
    }else
        toolButtonAdd->setEnabled(true);

    groupBoxProp->setEnabled(flag);
    toolButtonCondAdd->setEnabled(flag);
    toolButtonCondDel->setEnabled(flag);
    m_conditionModel->setEditable(flag);
    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEdit->setDisabled(flag);
}

void FilterPropWidget::submit()
{
    QModelIndex rootIndex = m_mapper->rootIndex();
    QModelIndex srcIndex = m_model->index(
                m_mapper->currentIndex(),0,m_mapper->rootIndex());

    for (int row=0; row < m_model->rowCount(rootIndex); row++){
        QModelIndex childIndex = m_model->index(row, m_model->columnDisplayedAttr(
                                                    DBFILTERXML::FILTER,
                                                    DBFILTERXML::NAME),
                                                rootIndex);
        if (childIndex.data(Qt::UserRole) == DBFILTERXML::FILTER)
            if (lineEditName->text() == childIndex.data() &&
                    srcIndex != childIndex.sibling(row,0)) {
                QMessageBox::warning(this,tr("Предуреждение"),
                                     tr("Фильтр с таким именем уже существует"));
                return;
            }
    }

    m_mapper->submit();
    m_conditionModel->submitAll();
    removeEmpty();
    edit(false);

    emit dataChanged(srcIndex);
}

void FilterPropWidget::revert()
{
    m_mapper->revert();
    m_conditionModel->revertAll();
    removeEmpty();
    edit(false);
}

void FilterPropWidget::rowsRemoved(const QModelIndex &index, int start, int end)
{
    if (index == m_mapper->rootIndex()){
        if (m_oldIndex > end)
            m_oldIndex = m_oldIndex - end-start-1;
        else if (m_oldIndex >= start && m_oldIndex <= end){
            m_oldIndex = -1;
        }
    }

    if (index == m_mapper->rootIndex() && m_mapper->currentIndex()==-1
            && m_oldIndex <0)
        emit dataRemoved(QModelIndex());
}


QVariant FilterPropWidget::modelData(const QString &tag, const QString &attr,
                                     const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}

void FilterPropWidget::removeEmpty()
{
    if (lineEditName->text().isEmpty()){
        if (m_oldIndex>=0){
            m_model->removeRow(m_mapper->currentIndex(),
                               m_mapper->rootIndex());
            setCurrent(m_mapper->rootIndex().child(m_oldIndex,0));
            m_oldIndex = -1;
        }else {
            remove();
            return;
        }
    }
}
