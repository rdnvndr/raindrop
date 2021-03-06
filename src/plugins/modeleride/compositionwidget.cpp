#include "compositionwidget.h"

#include <QHeaderView>
#include <QMessageBox>

#include <metadatamodel/dbxmlstruct.h>

#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

CompositionWidget::CompositionWidget(QWidget *parent) :
    AbstractItemWidget(parent)
{
    setupUi(this);

    QStringList tags;
    tags << DBCOMPXML::COMP;
    proxyModel()->setAttributeTags(tags);

    setItemView(tableViewComp);

    connect(toolButtonAdd, &QToolButton::clicked,
            this, &CompositionWidget::add);
    connect(toolButtonDelete, &QToolButton::clicked,
            this, &CompositionWidget::remove);

    connect(pushButtonSave, &QToolButton::clicked,
            this, &CompositionWidget::submit);
    connect(pushButtonCancel, &QToolButton::clicked,
            this, &CompositionWidget::revert);

    connect(toolButtonEdit, &QToolButton::clicked, this,
            static_cast<void (AbstractItemWidget::*)()>(&AbstractItemWidget::edit));
    connect(checkBoxInInherited, &QToolButton::clicked,
            this, &CompositionWidget::showParent);

    connect(toolButtonDown, &QToolButton::clicked,
            this, &CompositionWidget::down);
    connect(toolButtonUp, &QToolButton::clicked,
            this, &CompositionWidget::up);
}

CompositionWidget::~CompositionWidget()
{

}

void CompositionWidget::setModel(TreeXmlHashModel *model)
{
    AbstractItemWidget::setModel(model);

    proxyModel()->setSourceModel(model);
    proxyModel()->setHeaderData(0, Qt::Horizontal, tr("Состав"));
    proxyModel()->setHeaderData(1, Qt::Horizontal, tr("Псевдоним"));
    proxyModel()->setHeaderData(2, Qt::Horizontal, tr("Исходный класс"));
    proxyModel()->setHeaderData(3, Qt::Horizontal, tr("Входящий класс"));
    proxyModel()->setHeaderData(4, Qt::Horizontal, tr("Представление"));
    proxyModel()->setHeaderData(5, Qt::Horizontal, tr("Композиция"));
    proxyModel()->setHeaderData(6, Qt::Horizontal, tr("Описание включения"));
    proxyModel()->setHeaderData(7, Qt::Horizontal, tr("Описание вхождения"));
    proxyModel()->setHeaderData(8, Qt::Horizontal, tr("Индетификатор"));
    proxyModel()->setColumnCount(9);
    proxyModel()->setDynamicSortFilter(true);

    tableViewComp->setModel(proxyModel());
    tableViewComp->setColumnHidden(8,true);

    QSortFilterProxyModel *classFilterModel = new QSortFilterProxyModel(this);
    classFilterModel->setFilterKeyColumn(0);
    classFilterModel->setFilterRole(TreeXmlModel::TagRole);
    classFilterModel->setFilterRegExp(DBCLASSXML::CLASS + "|" +
                                      DBMODELXML::MODEL + "|" +
                                      DBCLASSLISTXML::CLASSLIST);
    classFilterModel->setSourceModel(model);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);

    comboBoxDestClass->setModel(classFilterModel);
    comboBoxDestClass->setRootModelIndex(childIdx(0,0,classFilterModel->index(0,0)));
    comboBoxDestClass->setIndexColumn(model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                                 DBCLASSXML::ID));

    comboBoxLinkClass->setModel(classFilterModel);
    comboBoxLinkClass->setRootModelIndex(childIdx(0,0,classFilterModel->index(0,0)));
    comboBoxLinkClass->setIndexColumn(model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                                 DBCLASSXML::ID));

    dataMapper()->setModel(proxyModel());
    dataMapper()->addMapping(lineEditAlias,
                             model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                        DBCOMPXML::ALIAS));
    dataMapper()->addMapping(checkBoxDestIsView,
                             model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                        DBCOMPXML::ISVIEW));
    dataMapper()->addMapping(checkBoxIsComp,
                             model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                        DBCOMPXML::ISCOMP));
    dataMapper()->addMapping(lineEditDirectDesc,
                             model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                        DBCOMPXML::DIRECTDESCRIPTION));
    dataMapper()->addMapping(lineEditInverseDesc,
                             model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                        DBCOMPXML::INVERSEDESCRIPTION));
    dataMapper()->addMapping(lineEditSrcClass,
                             model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                        DBCOMPXML::PARENT));
    dataMapper()->addMapping(comboBoxDestClass,
                             model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                        DBCOMPXML::CLASS));
    dataMapper()->addMapping(comboBoxLinkClass,
                             model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                        DBCOMPXML::LINKCLASS));
}


bool CompositionWidget::add()
{
    if (AbstractItemWidget::add(DBCOMPXML::COMP)){
        comboBoxDestClass->setCurrentIndex(-1);
        comboBoxDestClass->setDisplayText("");
        comboBoxLinkClass->setCurrentIndex(-1);
        comboBoxLinkClass->setDisplayText("");
        return true;
    }
    return false;
}

void CompositionWidget::edit(bool flag)
{
    QModelIndex currentIndex = tableViewComp->currentIndex();

    if (flag==false){
        QModelIndex mapIndex = proxyModel()->index(
                    dataMapper()->currentIndex(),
                    model()->columnDisplayedAttr(DBCOMPXML::COMP,DBCOMPXML::LINKCLASS),
                    dataMapper()->rootIndex());
        if (mapIndex.data().toString().isEmpty())
            proxyModel()->removeRow(dataMapper()->currentIndex(),
                                    dataMapper()->rootIndex());
    }

    if (!currentIndex.isValid()){
        flag = false;
        toolButtonDelete->setEnabled(false);
        toolButtonEdit->setEnabled(false);
        toolButtonAdd->setEnabled(true);
    } else {
        if (isEdit()==flag)
            return;
        toolButtonEdit->setDisabled(flag);
        toolButtonDelete->setDisabled(flag);
        toolButtonAdd->setDisabled(flag);
    }

    groupBoxProp->setEnabled(flag);
    pushButtonSave->setEnabled(flag);
    pushButtonCancel->setEnabled(flag);

}

void CompositionWidget::submit()
{
    QModelIndex rootIndex = tableViewComp->rootIndex();
    QModelIndex srcIndex = childIdx(dataMapper()->currentIndex(),0,dataMapper()->rootIndex());
    qint32 nameColumn = model()->columnDisplayedAttr(DBCOMPXML::COMP, DBCOMPXML::LINKCLASS);

    qint32 row = 0;
    QModelIndex childIndex = proxyModel()->index(row, nameColumn, rootIndex);
    while (childIndex.isValid())
    {
        if (comboBoxLinkClass->currentText() == childIndex.data()
                && childIndex.sibling(row,0)!=srcIndex) {
            QMessageBox::warning(this,tr("Предуреждение"),
                                 tr("Состав с таким именем уже существует"));
            return;
        }
        childIndex = proxyModel()->index(++row, nameColumn, rootIndex);
    }
    AbstractItemWidget::submit();

}

bool CompositionWidget::isEdit()
{
     return groupBoxProp->isEnabled();
}

void CompositionWidget::showParent(bool flag)
{
    AbstractItemWidget::showParent(flag, DBCLASSXML::CLASS, DBCLASSXML::ID,
                                   DBCOMPXML::COMP, DBCOMPXML::PARENT);
}

}}
