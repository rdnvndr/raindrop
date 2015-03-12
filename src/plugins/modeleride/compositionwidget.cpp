#include "compositionwidget.h"

#include <QHeaderView>
#include <QMessageBox>

#include <metadatamodel/dbxmlstruct.h>

#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;

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

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDelete,SIGNAL(clicked()),this,SLOT(remove()));

    connect(pushButtonSave,SIGNAL(clicked()),this,SLOT(submit()));
    connect(pushButtonCancel,SIGNAL(clicked()),this,SLOT(revert()));

    connect(toolButtonEdit,SIGNAL(clicked()),this,SLOT(edit()));
    connect(checkBoxInInherited,SIGNAL(clicked(bool)),this,SLOT(showParent(bool)));

    connect(toolButtonDown,SIGNAL(clicked()),this,SLOT(down()));
    connect(toolButtonUp,SIGNAL(clicked()),this,SLOT(up()));
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

    QSortFilterProxyModel* classFilterModel = new QSortFilterProxyModel(this);
    classFilterModel->setFilterKeyColumn(0);
    classFilterModel->setFilterRole(TreeXmlModel::TagRole);
    classFilterModel->setFilterRegExp(DBCLASSXML::CLASS + "|" +
                                      DBMODELXML::MODEL + "|" +
                                      DBCLASSLISTXML::CLASSLIST);
    classFilterModel->setSourceModel(model);
    classFilterModel->setDynamicSortFilter(true);
    classFilterModel->sort(0);

    comboBoxDestClass->setModel(classFilterModel);
    comboBoxDestClass->setRootModelIndex(classFilterModel->index(0,0).child(0,0));
    comboBoxDestClass->setIndexColumn(model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                                 DBCLASSXML::ID));

    comboBoxLinkClass->setModel(classFilterModel);
    comboBoxLinkClass->setRootModelIndex(classFilterModel->index(0,0).child(0,0));
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


void CompositionWidget::add()
{
    if (AbstractItemWidget::add(DBCOMPXML::COMP)){
        comboBoxDestClass->setCurrentIndex(-1);
        comboBoxDestClass->setDisplayText("");
        comboBoxLinkClass->setCurrentIndex(-1);
        comboBoxLinkClass->setDisplayText("");
    }
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
    QModelIndex srcIndex = dataMapper()->rootIndex().child(dataMapper()->currentIndex(),0);
    int nameColumn = model()->columnDisplayedAttr(DBCOMPXML::COMP, DBCOMPXML::LINKCLASS);

    int row = 0;
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

void CompositionWidget::up()
{
    QPersistentModelIndex index = tableViewComp->currentIndex();
    QPersistentModelIndex srcIndex  = proxyModel()->mapToSource(index);
    QPersistentModelIndex srcParent = srcIndex.parent();
    int row = proxyModel()->mapToSource(index.sibling(index.row()-1,0)).row();
    if (model()->moveIndex(srcIndex,srcParent,row)) {
        if (row >= 0)
            index = tableViewComp->currentIndex().sibling(index.row()-2,0);
        else
            index = index.sibling(0,0);
        if (model()->removeRow(srcIndex.row(),srcIndex.parent()))
            setCurrent(index);
    }
}

void CompositionWidget::down()
{
    QPersistentModelIndex index = tableViewComp->currentIndex();
    QPersistentModelIndex srcIndex  = proxyModel()->mapToSource(index);
    QPersistentModelIndex srcParent = srcIndex.parent();
    QModelIndex indexNew = index.sibling(index.row()+2,0);
    int row = (indexNew.isValid()) ? proxyModel()->mapToSource(indexNew).row()
                                   : model()->rowCount(srcParent,
                                                       model()->tagsFilter(),
                                                       QStringList());
    if (model()->moveIndex(srcIndex,srcParent,row)) {
        index = tableViewComp->currentIndex().sibling(index.row()+2,0);
        if (model()->removeRow(srcIndex.row(),srcIndex.parent()))
            setCurrent(index);
    }
}

void CompositionWidget::showParent(bool flag)
{
    proxyModel()->setFilterRole(Qt::EditRole);

    if (flag==true){
        proxyModel()->setFilterRegExp("");
    } else {
        QModelIndex index = tableViewComp->rootIndex();
        QString className = modelData(DBCLASSXML::CLASS,
                                      DBCLASSXML::ID,
                                      index).toString();
        className.replace("{","\\{");
        className.replace("}","\\}");
        if (className.isEmpty()){
            proxyModel()->setFilterRegExp("\\S*");
        }else
            proxyModel()->setFilterRegExp(className);
    }
    proxyModel()->setFilterKeyColumn(model()->columnDisplayedAttr(DBCOMPXML::COMP,
                                                                DBCOMPXML::PARENT));
}

}}
