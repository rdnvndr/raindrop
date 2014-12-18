#include "refitemwidget.h"
#include "dbxmlstruct.h"
#include "refitemdelegate.h"
#include <QMessageBox>

RefItemWidget::RefItemWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_refModel = new RefProxyModel();
    m_refModel->setHiddenRow(true);

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonAddIn,SIGNAL(clicked()),this,SLOT(addChild()));
    connect(toolButtonDelete,SIGNAL(clicked()),this,SLOT(remove()));

    treeView->setItemDelegate(new RefItemDelegate());
}

RefItemWidget::~RefItemWidget()
{
    delete m_refModel;
}

void RefItemWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    m_refModel->setSourceModel(m_model);
    m_refModel->setHeaderData(0,  Qt::Horizontal, tr("Псевдоним"));
    m_refModel->setHeaderData(1,  Qt::Horizontal, tr("Класс/Фильтр"));
    m_refModel->setHeaderData(2,  Qt::Horizontal, tr("Родитель"));
    m_refModel->setHeaderData(3,  Qt::Horizontal, tr("Идентификатор"));
    treeView->setModel(m_refModel);

    for (int column = 2; column < m_refModel->columnCount(); column++)
        treeView->setColumnHidden(column,true);
    treeView->setColumnWidth(0,treeView->size().width()/3);
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
    QModelIndex srcIndex = treeView->currentIndex().parent();
    if (!srcIndex.isValid())
        srcIndex = treeView->rootIndex();

    QModelIndex index = m_refModel->insertLastRows(0,1,srcIndex);
    if (index.isValid()) {
        if (srcIndex.data(TreeXmlModel::TagRole)==DBLINKTOCLASSXML::LINKTOCLASS) {
            m_refModel->setData(index, DBLINKTOFILTERXML::LINKTOFILTER, TreeXmlModel::TagRole);
        } else {
            m_refModel->setData(index, DBLINKTOCLASSXML::LINKTOCLASS, TreeXmlModel::TagRole);
        }
        edit(true);
        treeView->setCurrentIndex(index);
    }
}

void RefItemWidget::addChild()
{
    QModelIndex srcIndex = treeView->currentIndex();
    if (!srcIndex.isValid())
        srcIndex = treeView->rootIndex();
    QModelIndex index = m_refModel->insertLastRows(0,1,srcIndex);
    if (index.isValid()) {
        if (srcIndex.data(TreeXmlModel::TagRole)==DBLINKTOCLASSXML::LINKTOCLASS) {
            m_refModel->setData(index, DBLINKTOFILTERXML::LINKTOFILTER, TreeXmlModel::TagRole);
        } else {
            m_refModel->setData(index, DBLINKTOCLASSXML::LINKTOCLASS, TreeXmlModel::TagRole);
        }
        edit(true);
        treeView->setCurrentIndex(index);
    }
}

void RefItemWidget::remove()
{
    QModelIndex curIndex = treeView->currentIndex();
    QModelIndex srcIndex = curIndex.parent();
    if (srcIndex.isValid() && curIndex.isValid()){
        if (!isRemove(curIndex))
            return;
        treeView->setCurrentIndex(QModelIndex());
        m_refModel->removeRow(curIndex.row(),srcIndex);
        treeView->setModel(m_refModel);
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить элемент справочника, поскольку он не выбран."));
}

void RefItemWidget::submit()
{
    edit(false);
    m_refModel->submitAll();
}

void RefItemWidget::edit(bool flag)
{

    treeView->setCurrentIndex(QModelIndex());

    toolButtonAdd->setEnabled(flag);
    toolButtonAddIn->setEnabled(flag);
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
    QModelIndex rootIndex = m_refModel->mapToSource(treeView->rootIndex());
    if (rootIndex == index)
        return;

    treeView->setRootIndex(m_refModel->mapFromSource(index));

    emit proxyIndexChanged(treeView->rootIndex());
}

