#include "refitemwidget.h"
#include "unitdelegate.h"
#include "dbxmlstruct.h"

#include <QMessageBox>

RefItemWidget::RefItemWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_refModel = new ModifyProxyModel();
    m_refModel->setHiddenRow(true);

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDelete,SIGNAL(clicked()),this,SLOT(remove()));
    tableView->setItemDelegate(new UnitDelegate());
}

RefItemWidget::~RefItemWidget()
{
    delete tableView->itemDelegate();
    delete m_refModel;
}

void RefItemWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    m_refModel->setSourceModel(m_model);
    m_refModel->setHeaderData(0,  Qt::Horizontal, tr("Наименование"));
    m_refModel->setHeaderData(1,  Qt::Horizontal, tr("Класс/Фильтр"));
    m_refModel->setHeaderData(2,  Qt::Horizontal, tr("Родитель"));
    m_refModel->setHeaderData(3,  Qt::Horizontal, tr("Идентификатор"));
    tableView->setModel(m_refModel);

    for (int column = 2; column < 16; column++)
        tableView->setColumnHidden(column,true);
}

ModifyProxyModel *RefItemWidget::proxyModel()
{
    return m_refModel;
}

bool RefItemWidget::isRemove(const QModelIndex &srcIndex)
{
    const ModifyProxyModel* modifyModel = dynamic_cast<const ModifyProxyModel*>(srcIndex.model());
    const TreeXmlHashModel *model = (modifyModel)?
                dynamic_cast<const TreeXmlHashModel*>(modifyModel->sourceModel())
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

void RefItemWidget::add()
{
    QModelIndex srcIndex = tableView->rootIndex();
    QModelIndex index = m_refModel->insertLastRows(0,1,srcIndex);
    if (index.isValid()) {
        m_refModel->setData(index, DBLINKTOCLASSXML::LINKTOCLASS, TreeXmlModel::TagRole);
        m_refModel->setData(index, QIcon(":/modeleride"), Qt::DecorationRole);
        tableView->setCurrentIndex(index);
        edit(true);
    }
}

void RefItemWidget::remove()
{

    QModelIndex srcIndex = tableView->rootIndex();
    QModelIndex curIndex = tableView->currentIndex();
    if (srcIndex.isValid() && curIndex.isValid()){
        if (!isRemove(curIndex))
            return;
        tableView->setCurrentIndex(tableView->rootIndex());
        m_refModel->removeRow(curIndex.row(),srcIndex);
        tableView->setModel(m_refModel);
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить ЕИ, поскольку нет выбраных ЕИ."));
}

void RefItemWidget::submit()
{
    edit(false);
    m_refModel->submitAll();
}

void RefItemWidget::edit(bool flag)
{
    if (flag == false)
        tableView->setCurrentIndex(tableView->rootIndex());

    toolButtonAdd->setEnabled(flag);
    toolButtonDelete->setEnabled(flag);
    m_refModel->setEditable(flag);
}

void RefItemWidget::revert()
{
    m_refModel->revertAll();
    edit(false);
}

void RefItemWidget::setRootIndex(const QModelIndex &index)
{
    QModelIndex rootIndex = m_refModel->mapToSource(tableView->rootIndex());
    if (rootIndex == index)
        return;

    tableView->setRootIndex(m_refModel->mapFromSource(index));
    emit proxyIndexChanged(tableView->rootIndex());
}

