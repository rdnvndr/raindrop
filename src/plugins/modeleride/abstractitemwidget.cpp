#include "abstractitemwidget.h"

#include <QMessageBox>

#include "xmldelegate.h"

namespace RTPTechGroup {
namespace ModelerIde {

AbstractItemWidget::AbstractItemWidget(QWidget *parent) : QWidget(parent)
{
    m_tableModel = new TableXMLProxyModel();

    m_mapper = new QDataWidgetMapper();
    m_mapper->setItemDelegate(new XmlDelegate(this));
    m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

AbstractItemWidget::~AbstractItemWidget()
{
    delete m_mapper;
    delete m_tableModel;
}

void AbstractItemWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;
    m_tableModel->setSourceModel(m_model);
    m_tableView->setModel(m_tableModel);
    m_mapper->setModel(m_tableModel);
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

void AbstractItemWidget::setTableView(QTableView *tableView)
{
    m_tableView = tableView;

    m_tableView->setItemDelegate(new XmlDelegate(this));
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(m_tableView,SIGNAL(clicked(QModelIndex)),
            this,SLOT(setCurrent(QModelIndex)));
}

QTableView *AbstractItemWidget::tableView()
{
    return m_tableView;
}

TableXMLProxyModel *AbstractItemWidget::tableModel()
{
    return m_tableModel;
}

QDataWidgetMapper *AbstractItemWidget::dataMapper()
{
    return m_mapper;
}

bool AbstractItemWidget::add(const QString &tag)
{
    QModelIndex srcIndex = m_tableModel->mapToSource(m_tableView->rootIndex());
    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex, tag);
    if (srcCurrentIndex.isValid()){
        this->setCurrent(m_tableModel->mapFromSource(srcCurrentIndex));
        edit(true);
        return true;
    }

    return false;
}

void AbstractItemWidget::remove()
{
    edit(false);
    QModelIndex srcIndex = m_tableModel->mapToSource(m_tableView->rootIndex());
    QModelIndex curIndex = m_tableModel->mapToSource(m_tableView->currentIndex());
    if (srcIndex.isValid() && curIndex.isValid()){
        emit dataRemoved(srcIndex);
        m_model->removeRow(curIndex.row(),srcIndex);
        this->setCurrent(m_tableView->currentIndex());
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить, поскольку ничего не выбрано."));
}

void AbstractItemWidget::setCurrent(const QModelIndex &index)
{
    edit(false);
    if (!index.isValid())
        return;

    m_tableView->setCurrentIndex(index);
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
    this->setCurrent(m_tableView->currentIndex());
    edit(false);
}

void AbstractItemWidget::setRootIndex(QModelIndex index)
{
    QModelIndex rootIndex = m_tableModel->mapToSource(m_tableView->rootIndex());
    if (rootIndex == index)
        return;

    m_tableModel->setFilterIndex(index);
    m_tableModel->setSourceModel(m_model);

    m_tableView->setRootIndex(m_tableModel->mapFromSource(index));
    m_tableView->setCurrentIndex(m_tableView->rootIndex().child(0,0));

    m_mapper->setRootIndex(m_tableModel->mapFromSource(index));

    this->setCurrent(m_tableView->rootIndex().child(0,0));
}

}}
