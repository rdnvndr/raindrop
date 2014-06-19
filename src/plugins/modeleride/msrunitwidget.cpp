#include "msrunitwidget.h"
#include "dbxmlstruct.h"
#include "xmldelegate.h"
#include <QStringListModel>
#include <QTreeView>
#include <QMessageBox>
#include "treefilterproxymodel.h"
#include <QDebug>

MsrUnitWidget::MsrUnitWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_unitModel = new ModifyProxyModel();
    m_unitModel->setEditable(true);
    m_unitModel->setHiddenRow(true);

    connect(toolButtonAddUnit,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDeleteUnit,SIGNAL(clicked()),this,SLOT(remove()));
}

MsrUnitWidget::~MsrUnitWidget()
{
    delete m_unitModel;
}

void MsrUnitWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    m_unitModel->setSourceModel(m_model);
    m_unitModel->setHeaderData(0,  Qt::Horizontal, tr("Обозначение"));
    m_unitModel->setHeaderData(1,  Qt::Horizontal, tr("Условное обозначение"));
    m_unitModel->setHeaderData(2,  Qt::Horizontal, tr("Код ОКЕИ"));
    m_unitModel->setHeaderData(3,  Qt::Horizontal, tr("Коэффициент"));
    m_unitModel->setHeaderData(4,  Qt::Horizontal, tr("Разница"));
    m_unitModel->setHeaderData(5,  Qt::Horizontal, tr("Сущность ЕИ"));
    m_unitModel->setHeaderData(6,  Qt::Horizontal, tr("Индентификатор"));
    tableViewUnit->setModel(m_unitModel);

    for (int column = 5; column < 14; column++)
        tableViewUnit->setColumnHidden(column,true);
}

ModifyProxyModel *MsrUnitWidget::proxyModel()
{
    return m_unitModel;
}

void MsrUnitWidget::add()
{
    QModelIndex srcIndex = tableViewUnit->rootIndex();
    if (m_unitModel->insertRow(0,srcIndex)) {
        QModelIndex index = m_unitModel->lastInsertRow();
        m_unitModel->setData(index, DBMSRUNITXML::UNIT, Qt::UserRole);
        m_unitModel->setData(index, QIcon(":/unit"), Qt::DecorationRole);
        tableViewUnit->setCurrentIndex(index);
    }
}

void MsrUnitWidget::remove()
{    

    QModelIndex srcIndex = tableViewUnit->rootIndex();
    QModelIndex curIndex = tableViewUnit->currentIndex();
    if (srcIndex.isValid() && curIndex.isValid()){
        tableViewUnit->setCurrentIndex(tableViewUnit->rootIndex());
        m_unitModel->removeRow(curIndex.row(),srcIndex);
        tableViewUnit->setModel(m_unitModel);
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить ЕИ, поскольку нет выбраных ЕИ."));
}

void MsrUnitWidget::submit()
{
    edit(false);
    m_unitModel->submitAll();
}

void MsrUnitWidget::edit(bool flag)
{
    if (groupBoxUnit->isEnabled()== flag)
        return;

    if (flag == false)
        tableViewUnit->setCurrentIndex(tableViewUnit->rootIndex());
    groupBoxUnit->setEnabled(flag);
}

void MsrUnitWidget::revert()
{
    m_unitModel->revertAll();
    edit(false);
}

void MsrUnitWidget::setRootIndex(const QModelIndex &index)
{
    QModelIndex rootIndex = m_unitModel->mapToSource(tableViewUnit->rootIndex());
    if (rootIndex == index)
        return;

    tableViewUnit->setRootIndex(m_unitModel->mapFromSource(index));
    emit proxyIndexChanged(tableViewUnit->rootIndex());
}

