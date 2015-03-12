#include "abstractlisteditorwidget.h"

#include <QMessageBox>
#include <metadatamodel/classmodelxml.h>

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {


AbstractListEditorWidget::AbstractListEditorWidget(QWidget *parent) : QWidget(parent)
{
    m_proxyModel = new ModifyProxyModel();
    m_proxyModel->setHiddenRow(true);
}

AbstractListEditorWidget::~AbstractListEditorWidget()
{
    delete m_proxyModel;
}

void AbstractListEditorWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;
    m_proxyModel->setSourceModel(m_model);
    m_tableView->setModel(m_proxyModel);
}

QAbstractProxyModel *AbstractListEditorWidget::proxyModel()
{
    return m_proxyModel;
}

bool AbstractListEditorWidget::isRemove(const QModelIndex &proxyIndex)
{
    return true;
}

void AbstractListEditorWidget::setTableView(QTableView *tableView)
{
    m_tableView = tableView;
}

QTableView *AbstractListEditorWidget::tableView()
{
    return m_tableView;
}

bool AbstractListEditorWidget::add(const QString& tag)
{
    QModelIndex srcIndex = m_tableView->rootIndex();
    QModelIndex index = m_proxyModel->insertLastRows(0,1,srcIndex);
    if (index.isValid()) {
        m_proxyModel->setData(index,tag, TreeXmlModel::TagRole);
        m_tableView->setCurrentIndex(index);
        edit(true);

        return true;
    }
    return false;
}

void AbstractListEditorWidget::remove()
{

    QModelIndex srcIndex = m_tableView->rootIndex();
    QModelIndex curIndex = m_tableView->currentIndex();
    if (srcIndex.isValid() && curIndex.isValid()){
        if (!isRemove(curIndex))
            return;
        m_tableView->setCurrentIndex(m_tableView->rootIndex());
        m_proxyModel->removeRow(curIndex.row(),srcIndex);
        m_tableView->setModel(m_proxyModel);
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить значение списка, поскольку нет выбраных значений."));
}

void AbstractListEditorWidget::submit()
{
    edit(false);
    m_proxyModel->submitAll();
}

void AbstractListEditorWidget::edit(bool flag)
{
    if (flag == false)
        m_tableView->setCurrentIndex(m_tableView->rootIndex());

    m_proxyModel->setEditable(flag);
}

void AbstractListEditorWidget::revert()
{
    m_proxyModel->revertAll();
    edit(false);
}

void AbstractListEditorWidget::setRootIndex(const QModelIndex &index)
{
    QModelIndex rootIndex = m_proxyModel->mapToSource(m_tableView->rootIndex());
    if (rootIndex == index)
        return;

    m_tableView->setRootIndex(m_proxyModel->mapFromSource(index));
    emit proxyIndexChanged(m_tableView->rootIndex());
}

}}
