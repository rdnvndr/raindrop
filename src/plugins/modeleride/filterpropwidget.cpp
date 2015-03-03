#include "filterpropwidget.h"
#include <QMessageBox>
#include <QMenu>
#include <QSortFilterProxyModel>
#include <QToolTip>
#include "conditiondelegate.h"
#include "xmldelegate.h"
#include <metadatamodel/dbxmlstruct.h>
#include "regexpvalidator.h"

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

FilterPropWidget::FilterPropWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9_]{0,26}|^[A-Za-z]{0}"));
    lineEditName->setValidator(validator);
    connect(validator,SIGNAL(stateChanged(QValidator::State)),
            this,SLOT(validateFilterName(QValidator::State)));

    menuAddCondition = new QMenu(tr("Добавить"), this);

    actionAddCondition = menuAddCondition->addAction(QIcon(":/expression"),
                                                     tr("Выражение"));
    actionAddSubCondition = menuAddCondition->addAction(QIcon(":/expression"),
                                                        tr("Подвыражение"));
    actionAddBlock = menuAddCondition->addAction(QIcon(":/block"),tr("Блок"));
    actionAddSubBlock = menuAddCondition->addAction(QIcon(":/block"),tr("Подблок"));

    toolButtonCondAdd->setMenu(menuAddCondition);

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_conditionModel = new ConditionProxyModel();
    treeViewCondition->setItemDelegate(new ConditionDelegate(this));

    connect(comboBoxDestClass,SIGNAL(currentIndexChanged(QString)),
            this,SLOT(changeDestClass(QString)));

    lineEditSrcClass->setReadOnly(true);

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDel,SIGNAL(clicked()),this,SLOT(revert()));
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

    m_oldIndex = QModelIndex();
}

FilterPropWidget::~FilterPropWidget()
{
    delete lineEditName->validator();
    delete m_conditionModel;
    delete m_mapper;
    delete actionAddBlock;
    delete actionAddCondition;
    delete actionAddSubBlock;
    delete actionAddSubCondition;
    delete menuAddCondition;
}

void FilterPropWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;
    connect(m_model,SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this,SLOT(rowsRemoved(QModelIndex,int,int)));

    m_mapper->setModel(m_model);


    QSortFilterProxyModel* classFilterModel = new QSortFilterProxyModel(this);
    classFilterModel->setFilterKeyColumn(0);
    classFilterModel->setFilterRole(TreeXmlModel::TagRole);
    classFilterModel->setSourceModel(m_model);
    classFilterModel->setFilterRegExp(DBCLASSXML::CLASS + "|" +
                                      DBMODELXML::MODEL + "|" +
                                      DBCLASSLISTXML::CLASSLIST);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);
    comboBoxDestClass->setModel(classFilterModel);
    comboBoxDestClass->setRootModelIndex(classFilterModel->index(0,0).child(0,0));
    comboBoxDestClass->setIndexColumn(m_model->columnDisplayedAttr(
                                          DBCLASSXML::CLASS, DBCLASSXML::ID));
    comboBoxDestClass->setCurrentModelIndex(
                classFilterModel->index(0,0).child(0,0).child(0,0));

    m_mapper->addMapping(lineEditName,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::NAME));
    m_mapper->addMapping(lineEditAlias,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::ALIAS));

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
    treeViewCondition->setColumnWidth(0, (treeViewCondition->size().width() - 200)/2);
    treeViewCondition->setColumnWidth(1, 75);
    treeViewCondition->setColumnWidth(2, (treeViewCondition->size().width() - 200)/2);
    treeViewCondition->setColumnWidth(3, 75);
}

void FilterPropWidget::add()
{
    m_oldIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
    QModelIndex srcIndex = m_oldIndex.parent();

    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex, DBFILTERXML::FILTER);
    if (srcCurrentIndex.isValid()){
        setCurrent(srcCurrentIndex);
        edit(true);
    }
}

void FilterPropWidget::remove()
{
    QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(), 0,
                                          m_mapper->rootIndex());
    m_mapper->revert();
    setCurrent(QModelIndex());

    m_model->removeRow(srcIndex.row(),srcIndex.parent());
    emit dataRemoved(srcIndex);
}

void FilterPropWidget::addSubCondition()
{
    QModelIndex parent = (treeViewCondition->currentIndex().isValid())
            ? treeViewCondition->currentIndex()
            : treeViewCondition->rootIndex();

    QModelIndex index = m_conditionModel->insertLastRows(0,1,parent);
    m_conditionModel->setData(index, DBCONDITIONXML::COND, TreeXmlModel::TagRole);
    m_conditionModel->setData(index.sibling(index.row(),3),tr("И"));
}

void FilterPropWidget::addCondition()
{
    QModelIndex parent = treeViewCondition->rootIndex();
    if (treeViewCondition->currentIndex().isValid()
            && parent != treeViewCondition->currentIndex())
        if (treeViewCondition->currentIndex().parent().isValid())
            parent = treeViewCondition->currentIndex().parent();


    QModelIndex index = m_conditionModel->insertLastRows(0,1,parent);
    m_conditionModel->setData(index, DBCONDITIONXML::COND, TreeXmlModel::TagRole);
    m_conditionModel->setData(index.sibling(index.row(),3),tr("И"));
}

void FilterPropWidget::removeCondition()
{
    QModelIndex index = treeViewCondition->currentIndex();
    if (index.isValid()) {
        m_conditionModel->removeRow(index.row(),index.parent());
        treeViewCondition->setCurrentIndex(treeViewCondition->rootIndex());
    } else
        QMessageBox::warning(this,tr("Предуреждение"),
                             tr("Удалить невозможно. Ничего не выбрано."));
}

void FilterPropWidget::addSubBlock()
{
    QModelIndex parent = (treeViewCondition->currentIndex().isValid())
            ? treeViewCondition->currentIndex() : treeViewCondition->rootIndex();

    QModelIndex index = m_conditionModel->insertLastRows(0,1,parent);
    m_conditionModel->setData(index, DBFILTERBLOCKXML::BLOCK, TreeXmlModel::TagRole);
    m_conditionModel->setData(index.sibling(index.row(),3),tr("И"));
}

void FilterPropWidget::addBlock()
{
    QModelIndex parent = treeViewCondition->rootIndex();
    if (treeViewCondition->currentIndex().isValid()
            && parent != treeViewCondition->currentIndex())
        if (treeViewCondition->currentIndex().parent().isValid())
            parent = treeViewCondition->currentIndex().parent();

    QModelIndex index = m_conditionModel->insertLastRows(0,1,parent);
    m_conditionModel->setData(index, DBFILTERBLOCKXML::BLOCK, TreeXmlModel::TagRole);
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

    ConditionDelegate *conditionDelegate =
            qobject_cast<ConditionDelegate *>(treeViewCondition->itemDelegate());
    conditionDelegate->setFirstIndex(index.parent());

    treeViewCondition->setRootIndex(m_conditionModel->mapFromSource(index));

    emit currentIndexChanged(index);
    changeDestClass(comboBoxDestClass->displayText());
}

bool FilterPropWidget::isEdit()
{
    return groupBoxProp->isEnabled();
}

void FilterPropWidget::edit(bool flag)
{
    if (isEdit()==flag)
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
    int filterNameColumn =
            m_model->columnDisplayedAttr(DBFILTERXML::FILTER, DBFILTERXML::NAME);
    int row=0;
    QModelIndex childIndex = m_model->index(row, filterNameColumn, rootIndex);
    while (childIndex.isValid())
    {
        if (childIndex.data(TreeXmlModel::TagRole) == DBFILTERXML::FILTER)
            if (lineEditName->text().toUpper() == childIndex.data().toString().toUpper()
                    && srcIndex != childIndex.sibling(row,0)) {
                QMessageBox::warning(this,tr("Предуреждение"),
                                     tr("Фильтр с таким именем уже существует"));
                return;
            }
         childIndex = m_model->index(++row, filterNameColumn, rootIndex);
    }

    m_mapper->submit();
    m_conditionModel->submitAll();

    edit(false);
    if (!removeEmpty())
        emit dataChanged(srcIndex);
}

void FilterPropWidget::revert()
{
    m_mapper->revert();
    m_conditionModel->revertAll();
    edit(false);
    removeEmpty();

}

void FilterPropWidget::rowsRemoved(const QModelIndex &index, int start, int end)
{
    Q_UNUSED (start)
    Q_UNUSED (end)
    if (index == m_mapper->rootIndex() && m_mapper->currentIndex()==-1)
        emit dataRemoved(QModelIndex());
}

void FilterPropWidget::changeDestClass(const QString &nameClass)
{
    QModelIndex index = m_model->indexHashAttr(
                DBCLASSXML::CLASS, DBCLASSXML::NAME, nameClass);

    ConditionDelegate *conditionDelegate =
            qobject_cast<ConditionDelegate *>(treeViewCondition->itemDelegate());

    if (index.isValid()) conditionDelegate->setSecondIndex(index);
}

void FilterPropWidget::validateFilterName(QValidator::State state) const
{
    if(state != QValidator::Acceptable)
        QToolTip::showText(lineEditName->mapToGlobal(QPoint(0,5)),
                           tr("Имя фильтра должно содержать только латинские\n"
                              "символы и цифры длиной не более 27 символов"));
    else
        QToolTip::hideText();
}

QVariant FilterPropWidget::modelData(const QString &tag, const QString &attr,
                                     const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}

bool FilterPropWidget::removeEmpty()
{
    if (lineEditName->text().isEmpty()){
        if (m_oldIndex.isValid()){
            QModelIndex srcIndex = m_model->index(m_mapper->currentIndex(),0,m_mapper->rootIndex());
            m_mapper->revert();
            setCurrent(m_oldIndex);
            m_model->removeRow(srcIndex.row(),srcIndex.parent());
            m_oldIndex = QModelIndex();
        }else {
            remove();
        }
        return true;
    }
    return false;
}

}}
