#include "abstractmodifywidget.h"

#include <QMessageBox>
#include <metadatamodel/classmodelxml.h>

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

AbstractModifyWidget::AbstractModifyWidget(QWidget *parent) : QWidget(parent)
{
    m_proxyModel = new ModifyProxyModel();
    m_proxyModel->setHiddenRow(true);
    m_model = nullptr;
    m_itemView = nullptr;
}

AbstractModifyWidget::~AbstractModifyWidget()
{
    delete m_proxyModel;
}

void AbstractModifyWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;
    m_proxyModel->setSourceModel(m_model);
    m_itemView->setModel(m_proxyModel);
}

TreeXmlHashModel *AbstractModifyWidget::model()
{
    return m_model;
}

ModifyProxyModel *AbstractModifyWidget::proxyModel()
{
    return m_proxyModel;
}

bool AbstractModifyWidget::isRemove(const QModelIndex &proxyIndex)
{
    Q_UNUSED(proxyIndex)

    return true;
}

void AbstractModifyWidget::setItemView(QAbstractItemView *tableView)
{
    m_itemView = tableView;
}

QAbstractItemView *AbstractModifyWidget::itemView()
{
    return m_itemView;
}

bool AbstractModifyWidget::add(const QString& tag)
{
    QModelIndex srcIndex = m_itemView->rootIndex();
    QModelIndex index = m_proxyModel->insertLastRows(0,1,srcIndex);
    if (index.isValid()) {
        m_proxyModel->setData(index,tag, TreeXmlModel::TagRole);
        m_itemView->setCurrentIndex(index);
        edit(true);

        return true;
    }
    return false;
}

void AbstractModifyWidget::remove()
{    
    QModelIndex curIndex = m_itemView->currentIndex();
    QModelIndex srcIndex = curIndex.parent();

    if (srcIndex.isValid() && curIndex.isValid() && m_itemView->rootIndex() != curIndex){
        if (!isRemove(curIndex))
            return;

        QPersistentModelIndex nextIndex = curIndex.sibling(curIndex.row()+1,0);
        if (!nextIndex.isValid()) nextIndex = curIndex.sibling(curIndex.row()-1,0);
        if (nextIndex.isValid())
            m_itemView->setCurrentIndex(nextIndex);
        else
            m_itemView->setCurrentIndex(m_itemView->rootIndex());

        m_proxyModel->removeRow(curIndex.row(),srcIndex);
        m_itemView->setModel(m_proxyModel);
    } else
        QMessageBox::warning(
            nullptr, tr("Предупреждение"),
            tr("Невозможно удалить значение списка, поскольку нет выбранных значений.")
        );
}

void AbstractModifyWidget::submit()
{
    edit(false);
    m_proxyModel->submitAll();
}

void AbstractModifyWidget::edit(bool flag)
{
    if (flag == false)
        m_itemView->setCurrentIndex(m_itemView->rootIndex());

    m_proxyModel->setEditable(flag);
}

void AbstractModifyWidget::edit()
{
    edit(true);
}

void AbstractModifyWidget::revert()
{
    m_proxyModel->revertAll();
    edit(false);
}

void AbstractModifyWidget::setRootIndex(const QModelIndex &index)
{
    QModelIndex rootIndex = m_proxyModel->mapToSource(m_itemView->rootIndex());
    if (rootIndex == index)
        return;

    m_itemView->setRootIndex(m_proxyModel->mapFromSource(index));
    emit proxyIndexChanged(m_itemView->rootIndex());
}

void AbstractModifyWidget::up()
{
    QPersistentModelIndex srcIndex = itemView()->currentIndex();
    QPersistentModelIndex dstIndex = srcIndex.sibling(srcIndex.row()-1,0);
    if (!dstIndex.isValid()) return;

    QVariant srcTag = srcIndex.sibling(srcIndex.row(), 0).data(TreeXmlModel::TagRole);
    QVariant dstTag = dstIndex.sibling(dstIndex.row(), 0).data(TreeXmlModel::TagRole);
    proxyModel()->setData(srcIndex.sibling(srcIndex.row(), 0), dstTag, TreeXmlModel::TagRole);
    proxyModel()->setData(dstIndex.sibling(dstIndex.row(), 0), srcTag, TreeXmlModel::TagRole);

    for (qint32 i = 0; i<proxyModel()->columnCount(srcIndex); ++i) {
        QVariant srcValue = srcIndex.sibling(srcIndex.row(), i).data(Qt::EditRole);
        QVariant dstValue = dstIndex.sibling(dstIndex.row(), i).data(Qt::EditRole);
        proxyModel()->setData(srcIndex.sibling(srcIndex.row(), i), dstValue);
        proxyModel()->setData(dstIndex.sibling(dstIndex.row(), i), srcValue);
    }
    itemView()->setCurrentIndex(dstIndex);
}

void AbstractModifyWidget::down()
{
    QPersistentModelIndex srcIndex = itemView()->currentIndex();
    QPersistentModelIndex dstIndex = srcIndex.sibling(srcIndex.row()+1,0);
    if (!dstIndex.isValid()) return;

    QVariant srcTag = srcIndex.sibling(srcIndex.row(), 0).data(TreeXmlModel::TagRole);
    QVariant dstTag = dstIndex.sibling(dstIndex.row(), 0).data(TreeXmlModel::TagRole);
    proxyModel()->setData(srcIndex.sibling(srcIndex.row(), 0), dstTag, TreeXmlModel::TagRole);
    proxyModel()->setData(dstIndex.sibling(dstIndex.row(), 0), srcTag, TreeXmlModel::TagRole);

    for (qint32 i = 0; i<proxyModel()->columnCount(srcIndex); ++i) {
        QVariant srcValue = srcIndex.sibling(srcIndex.row(), i).data(Qt::EditRole);
        QVariant dstValue = dstIndex.sibling(dstIndex.row(), i).data(Qt::EditRole);
        proxyModel()->setData(srcIndex.sibling(srcIndex.row(), i), dstValue);
        proxyModel()->setData(dstIndex.sibling(dstIndex.row(), i), srcValue);
    }
    itemView()->setCurrentIndex(dstIndex);
}

}}
