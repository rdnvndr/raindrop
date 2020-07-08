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
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

FilterPropWidget::FilterPropWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);

    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9_]{0,26}|^[A-Za-z]{0}"));
    lineEditName->setValidator(validator);
    connect(validator, &RegExpValidator::stateChanged,
            this, &FilterPropWidget::validateFilterName);

    menuAddCondition = new QMenu(tr("Добавить"), this);

    actionAddCondition = menuAddCondition->addAction(QIcon(":/expression"),
                                                     tr("Выражение"));
    actionAddSubCondition = menuAddCondition->addAction(QIcon(":/expression"),
                                                        tr("Подвыражение"));
    actionAddBlock = menuAddCondition->addAction(QIcon(":/block"),tr("Блок"));
    actionAddSubBlock = menuAddCondition->addAction(QIcon(":/block"),tr("Подблок"));

    toolButtonCondAdd->setMenu(menuAddCondition);

    m_conditionModel = new ConditionProxyModel();
    treeViewCondition->setItemDelegate(new ConditionDelegate(this));

    connect(comboBoxDestClass,
            static_cast<void (QComboBox::*)(int)>
            (&RTPTechGroup::Widgets::TreeComboBox::currentIndexChanged),
            this, &FilterPropWidget::changeDestClass);

    lineEditSrcClass->setReadOnly(true);

    connect(toolButtonAdd, &QToolButton::clicked, this, &FilterPropWidget::add);
    connect(toolButtonDel, &QToolButton::clicked, this, &FilterPropWidget::revert);
    connect(toolButtonDel, &QToolButton::clicked, this, &FilterPropWidget::remove);
    connect(pushButtonPropSave, &QPushButton::clicked,
            this, &FilterPropWidget::submit);
    connect(pushButtonPropCancel, &QPushButton::clicked,
            this, &FilterPropWidget::revert);
    connect(toolButtonEdit, &QToolButton::clicked, this,
            static_cast<void (AbstractEditorWidget::*)()>(&AbstractEditorWidget::edit));

    connect(actionAddCondition, &QAction::triggered,
            this, &FilterPropWidget::addCondition);
    connect(actionAddBlock, &QAction::triggered,
            this, &FilterPropWidget::addBlock);
    connect(actionAddSubCondition, &QAction::triggered,
            this, &FilterPropWidget::addSubCondition);
    connect(actionAddSubBlock, &QAction::triggered,
            this, &FilterPropWidget::addSubBlock);

    connect(toolButtonCondAdd, &QToolButton::clicked,
            toolButtonCondAdd, &QToolButton::showMenu);
    connect(toolButtonCondDel, &QToolButton::clicked,
            this, &FilterPropWidget::removeCondition);

}

FilterPropWidget::~FilterPropWidget()
{
    delete lineEditName->validator();
    delete m_conditionModel;
    delete actionAddBlock;
    delete actionAddCondition;
    delete actionAddSubBlock;
    delete actionAddSubCondition;
    delete menuAddCondition;
}

void FilterPropWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    QSortFilterProxyModel *classFilterModel = new QSortFilterProxyModel(this);
    classFilterModel->setFilterKeyColumn(0);
    classFilterModel->setFilterRole(TreeXmlModel::TagRole);
    classFilterModel->setSourceModel(model);
    classFilterModel->setFilterRegExp(DBCLASSXML::CLASS + "|" +
                                      DBMODELXML::MODEL + "|" +
                                      DBCLASSLISTXML::CLASSLIST);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);
    comboBoxDestClass->setModel(classFilterModel);
    comboBoxDestClass->setRootModelIndex(childIdx(0,0,classFilterModel->index(0,0)));
    comboBoxDestClass->setIndexColumn(model->columnDisplayedAttr(
                                          DBCLASSXML::CLASS, DBCLASSXML::ID));
    comboBoxDestClass->setCurrentModelIndex(
                childIdx(0,0,childIdx(0,0,classFilterModel->index(0,0))));

    dataMapper()->addMapping(lineEditName,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::NAME));
    dataMapper()->addMapping(lineEditAlias,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::ALIAS));

    dataMapper()->addMapping(lineEditDirectDesc,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::DIRECTDESCRIPTION));

    dataMapper()->addMapping(lineEditInverseDesc,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::INVERSEDESCRIPTION));
    dataMapper()->addMapping(lineEditSrcClass,
                         model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                   DBFILTERXML::PARENT));

    dataMapper()->addMapping(comboBoxDestClass,
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

bool FilterPropWidget::isEmpty()
{
    return lineEditName->text().isEmpty();
}

void FilterPropWidget::add()
{
    AbstractEditorWidget::add(DBFILTERXML::FILTER);
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
    m_conditionModel->setData(index, DBFILTERBLOCKXML::BLOCK,
                              TreeXmlModel::TagRole);
    m_conditionModel->setData(index.sibling(index.row(),3),tr("И"));
}

void FilterPropWidget::setCurrent(const QModelIndex &index)
{
    ConditionDelegate *conditionDelegate =
            qobject_cast<ConditionDelegate *>(treeViewCondition->itemDelegate());
    conditionDelegate->setFirstIndex(index.parent());

    treeViewCondition->setRootIndex(m_conditionModel->mapFromSource(index));
    changeDestClass(comboBoxDestClass->currentIndex());

    AbstractEditorWidget::setCurrent(index);
}

bool FilterPropWidget::isEdit()
{
    return groupBoxProp->isEnabled();
}

void FilterPropWidget::edit(bool flag)
{
    if (isEmpty()){
        toolButtonAdd->setDisabled(true);
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
    QModelIndex rootIndex = dataMapper()->rootIndex();
    QModelIndex srcIndex = model()->index(
                dataMapper()->currentIndex(),0,dataMapper()->rootIndex());
    qint32 filterNameColumn =
            model()->columnDisplayedAttr(DBFILTERXML::FILTER, DBFILTERXML::NAME);
    qint32 row=0;
    QModelIndex childIndex = model()->index(row, filterNameColumn, rootIndex);
    while (childIndex.isValid())
    {
        if (childIndex.data(TreeXmlModel::TagRole) == DBFILTERXML::FILTER)
            if (lineEditName->text().toUpper() == childIndex.data().toString().toUpper()
                    && srcIndex != childIndex.sibling(row,0)) {
                QMessageBox::warning(this,tr("Предуреждение"),
                                     tr("Фильтр с таким именем уже существует"));
                return;
            }
         childIndex = model()->index(++row, filterNameColumn, rootIndex);
    }

    AbstractEditorWidget::submit();
    m_conditionModel->submitAll();
}

void FilterPropWidget::revert()
{
    m_conditionModel->revertAll();
    AbstractEditorWidget::revert();
}

void FilterPropWidget::changeDestClass(const int &idx)
{
    QModelIndex index = model()->indexHashAttr(
                DBCLASSXML::CLASS, DBCLASSXML::NAME,
                comboBoxDestClass->itemText(idx));

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

}}
