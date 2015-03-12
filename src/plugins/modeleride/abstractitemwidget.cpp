#include "abstractitemwidget.h"

#include <QMessageBox>

#include "xmldelegate.h"

namespace RTPTechGroup {
namespace ModelerIde {

AbstractItemWidget::AbstractItemWidget(QWidget *parent) : QWidget(parent)
{
    m_proxyModel = new TableXMLProxyModel();

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

AbstractItemWidget::~AbstractItemWidget()
{
    delete m_mapper;
    delete m_proxyModel;
}

void AbstractItemWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;
    m_proxyModel->setSourceModel(m_model);
    m_itemView->setModel(m_proxyModel);
    m_mapper->setModel(m_proxyModel);
}

TreeXmlHashModel *AbstractItemWidget::model()
{
    return m_model;
}

QVariant AbstractItemWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}

void AbstractItemWidget::setItemView(QAbstractItemView *tableView)
{
    m_itemView = tableView;

    m_itemView->setItemDelegate(new XmlDelegate(this));
    m_itemView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(m_itemView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(setCurrent(QModelIndex)));
}

QAbstractItemView *AbstractItemWidget::itemView()
{
    return m_itemView;
}

TableXMLProxyModel *AbstractItemWidget::proxyModel()
{
    return m_proxyModel;
}

QDataWidgetMapper *AbstractItemWidget::dataMapper()
{
    return m_mapper;
}

bool AbstractItemWidget::add(const QString &tag)
{
    QModelIndex srcIndex = m_proxyModel->mapToSource(m_itemView->rootIndex());
    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex, tag);
    if (srcCurrentIndex.isValid()){
        this->setCurrent(m_proxyModel->mapFromSource(srcCurrentIndex));
        edit(true);
        return true;
    }

    return false;
}

void AbstractItemWidget::remove()
{
    edit(false);
    QModelIndex srcIndex = m_proxyModel->mapToSource(m_itemView->rootIndex());
    QModelIndex curIndex = m_proxyModel->mapToSource(m_itemView->currentIndex());
    if (srcIndex.isValid() && curIndex.isValid()){
        emit dataRemoved(srcIndex);
        m_model->removeRow(curIndex.row(),srcIndex);
        this->setCurrent(m_itemView->currentIndex());
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить, поскольку ничего не выбрано."));
}

void AbstractItemWidget::setCurrent(const QModelIndex &index)
{
    edit(false);
    if (!index.isValid())
        return;

    m_itemView->setCurrentIndex(index);
    m_mapper->setCurrentModelIndex(index);

    emit currentIndexChanged(index);
}

void AbstractItemWidget::submit()
{

    QModelIndex srcIndex = m_mapper->rootIndex().child(m_mapper->currentIndex(),0);
    m_mapper->submit();
    edit(false);

    emit dataChanged(srcIndex);
}

void AbstractItemWidget::revert()
{
    m_mapper->revert();
    this->setCurrent(m_itemView->currentIndex());
    edit(false);
}

void AbstractItemWidget::setRootIndex(QModelIndex index)
{
    QModelIndex rootIndex = m_proxyModel->mapToSource(m_itemView->rootIndex());
    if (rootIndex == index)
        return;

    m_proxyModel->setFilterIndex(index);
    m_proxyModel->setSourceModel(m_model);

    m_itemView->setRootIndex(m_proxyModel->mapFromSource(index));
    m_itemView->setCurrentIndex(m_itemView->rootIndex().child(0,0));

    m_mapper->setRootIndex(m_proxyModel->mapFromSource(index));

    this->setCurrent(m_itemView->rootIndex().child(0,0));
}

}}
