#include "lovvaluewidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "lovdelegate.h"
#include <QMessageBox>

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

LovValueWidget::LovValueWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_lovValueModel = new ModifyProxyModel();
    m_lovValueModel->setHiddenRow(true);

    connect(toolButtonAddLovValue,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDeleteLovValue,SIGNAL(clicked()),this,SLOT(remove()));

    tableViewLovValue->setItemDelegate(new LovDelegate());
}

LovValueWidget::~LovValueWidget()
{
    delete tableViewLovValue->itemDelegate();
    delete m_lovValueModel;
}

void LovValueWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    m_lovValueModel->setSourceModel(m_model);
    m_lovValueModel->setHeaderData(0,  Qt::Horizontal, tr("Наименование"));
    m_lovValueModel->setHeaderData(1,  Qt::Horizontal, tr("Значение"));
    m_lovValueModel->setHeaderData(2,  Qt::Horizontal, tr("Список значений"));
    m_lovValueModel->setHeaderData(3,  Qt::Horizontal, tr("Идентификатор"));
    tableViewLovValue->setModel(m_lovValueModel);

    for (int column = 2; column < 16; column++)
        tableViewLovValue->setColumnHidden(column,true);
}

QAbstractProxyModel *LovValueWidget::proxyModel()
{
    return m_lovValueModel;
}

bool LovValueWidget::isRemove(const QModelIndex &srcIndex)
{
    const QAbstractProxyModel* proxyModel = dynamic_cast<const QAbstractProxyModel*>(srcIndex.model());
    const TreeXmlHashModel *model = (proxyModel)?
                dynamic_cast<const TreeXmlHashModel*>(proxyModel->sourceModel())
              : dynamic_cast<const TreeXmlHashModel*>(srcIndex.model());

    if (!model)
        return false;

    bool success = true;
    QString msg;

    QString tag = srcIndex.data(TreeXmlModel::TagRole).toString();

    QString fieldId = model->uuidAttr(tag);
    if (fieldId.isEmpty())
        return true;

    QString guid =  srcIndex.sibling(srcIndex.row(),
                                     model->columnDisplayedAttr(
                                         tag,fieldId))
            .data().toString();

    foreach (TreeXmlHashModel::TagWithAttr tagWithAttr,
             model->fromRelation(tag))
    {
        int number = 0;

        QModelIndex linkIndex = model->indexHashAttr(
                    tagWithAttr.tag,
                    tagWithAttr.attr,
                    guid,
                    number
                    );

        while (linkIndex.isValid() && guid!="") {
            QModelIndex linkParent = linkIndex.parent();
            if (linkParent.sibling(linkIndex.parent().row(),0)!= srcIndex){
                QString parentName;
                QString name;

                if (linkParent.data(TreeXmlModel::TagRole) == DBCOMPXML::COMP)
                    parentName = tr(" принадлежащий составу ")
                            + linkParent.sibling(
                                linkParent.row(),
                                model->columnDisplayedAttr(
                                    DBCOMPXML::COMP,
                                    DBCOMPXML::NAME)
                                ).data().toString();
                else
                    parentName = tr(" принадлежащий классу ")
                            + linkParent.sibling(
                                linkParent.row(),
                                model->columnDisplayedAttr(
                                    DBCLASSXML::CLASS,
                                    DBCLASSXML::NAME)
                                ).data().toString();

                name = tr("атрибут ")
                        + linkIndex.sibling(linkIndex.row(),
                                            model->columnDisplayedAttr(
                                                DBATTRXML::ATTR,
                                                DBATTRXML::NAME)
                                            ).data().toString();

                msg += QString(tr("Необходимо удалить %1%2.\n\n")).
                        arg(name).arg(parentName);
                if (success)
                    success = false;
            }
            number++;
            linkIndex = model->indexHashAttr(
                        tagWithAttr.tag,
                        tagWithAttr.attr,
                        guid,
                        number
                        );
        }
    }
    if (!success) {
        QMessageBox msgBox;
        msgBox.setText(tr("Удаление данного объекта не воможно."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setDetailedText(msg);
        msgBox.setWindowTitle(tr("Предупреждение"));
        msgBox.exec();
    }
    return success;
}

void LovValueWidget::add()
{
    QModelIndex srcIndex = tableViewLovValue->rootIndex();
    QModelIndex index = m_lovValueModel->insertLastRows(0,1,srcIndex);
    if (index.isValid()) {
        m_lovValueModel->setData(index, DBLOVVALUEXML::LOVVALUE, TreeXmlModel::TagRole);
        m_lovValueModel->setData(index, QIcon(":/lovvalue"), Qt::DecorationRole);
        tableViewLovValue->setCurrentIndex(index);
        edit(true);
    }
}

void LovValueWidget::remove()
{

    QModelIndex srcIndex = tableViewLovValue->rootIndex();
    QModelIndex curIndex = tableViewLovValue->currentIndex();
    if (srcIndex.isValid() && curIndex.isValid()){
        if (!isRemove(curIndex))
            return;
        tableViewLovValue->setCurrentIndex(tableViewLovValue->rootIndex());
        m_lovValueModel->removeRow(curIndex.row(),srcIndex);
        tableViewLovValue->setModel(m_lovValueModel);
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить значение списка, поскольку нет выбраных значений."));
}

void LovValueWidget::submit()
{
    edit(false);
    m_lovValueModel->submitAll();
}

void LovValueWidget::edit(bool flag)
{
    if (flag == false)
        tableViewLovValue->setCurrentIndex(tableViewLovValue->rootIndex());

    toolButtonAddLovValue->setEnabled(flag);
    toolButtonDeleteLovValue->setEnabled(flag);
    m_lovValueModel->setEditable(flag);
}

void LovValueWidget::revert()
{
    m_lovValueModel->revertAll();
    edit(false);
}

void LovValueWidget::setRootIndex(const QModelIndex &index)
{
    QModelIndex rootIndex = m_lovValueModel->mapToSource(tableViewLovValue->rootIndex());
    if (rootIndex == index)
        return;

    tableViewLovValue->setRootIndex(m_lovValueModel->mapFromSource(index));
    emit proxyIndexChanged(tableViewLovValue->rootIndex());
}

}}
