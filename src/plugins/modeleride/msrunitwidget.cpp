#include "msrunitwidget.h"
#include "dbxmlstruct.h"
#include "xmldelegate.h"
#include <QStringListModel>
#include <QMessageBox>
#include "treefilterproxymodel.h"

MsrUnitWidget::MsrUnitWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_mapperUnit = new QDataWidgetMapper();
    m_mapperUnit->setItemDelegate(new XmlDelegate(this));
    m_mapperUnit->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_unitModel = new TableXMLProxyModel();
    QStringList tags;
    tags << DBMSRUNITXML::UNIT;
    m_unitModel->setAttributeTags(tags);

    tableViewUnit->setItemDelegate(new XmlDelegate(this));
    tableViewUnit->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(tableViewUnit,SIGNAL(clicked(QModelIndex)),
            this,SLOT(setCurrent(QModelIndex)));
    connect(toolButtonAddUnit,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDeleteUnit,SIGNAL(clicked()),this,SLOT(remove()));
}

MsrUnitWidget::~MsrUnitWidget()
{
    delete m_mapperUnit;
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
    m_unitModel->setDynamicSortFilter(true);

    tableViewUnit->setSortingEnabled(true);
    tableViewUnit->setModel(m_unitModel);
    tableViewUnit->sortByColumn(0,Qt::AscendingOrder);
    for (int column = 5; column < 14; column++)
        tableViewUnit->setColumnHidden(column,true);
    m_mapperUnit->setModel(m_unitModel);
}

void MsrUnitWidget::add()
{
    QModelIndex srcIndex = m_unitModel->mapToSource(tableViewUnit->rootIndex());
    m_model->setInsTagName(DBMSRUNITXML::UNIT);
    if (m_model->insertRow(0,srcIndex)){
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        this->setCurrent(m_unitModel->mapFromSource(srcCurrentIndex));
    }
}

void MsrUnitWidget::remove()
{
    QModelIndex srcIndex = m_unitModel->mapToSource(tableViewUnit->rootIndex());
    QModelIndex curIndex = m_unitModel->mapToSource(tableViewUnit->currentIndex());
    if (srcIndex.isValid() && curIndex.isValid()){
        emit dataRemoved(srcIndex);
        m_model->removeRow(curIndex.row(),srcIndex);
        this->setCurrent(tableViewUnit->currentIndex());
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить ЕИ, поскольку нет выбраных ЕИ."));
}

void MsrUnitWidget::setCurrent(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    tableViewUnit->setCurrentIndex(index);
    m_mapperUnit->setCurrentModelIndex(index);

    emit currentIndexChanged(index);
}

void MsrUnitWidget::submit()
{
    QModelIndex rootIndex = (tableViewUnit->rootIndex());
    QModelIndex srcIndex = m_mapperUnit->rootIndex().child(m_mapperUnit->currentIndex(),0);
    for (int row=0; row < m_unitModel->rowCount(rootIndex); row++){
        QModelIndex childIndex = m_unitModel->index(row,
                                                    m_model->columnDisplayedAttr(
                                                        DBMSRUNITXML::UNIT,
                                                        DBMSRUNITXML::DESIGNATION),
                                                    rootIndex);
    }
    edit(false);
    m_mapperUnit->submit();
    emit dataChanged(srcIndex);
}

void MsrUnitWidget::edit(bool flag)
{
    if (groupBoxUnit->isEnabled()== flag)
        return;

    if (flag == false)
        tableViewUnit->setCurrentIndex(QModelIndex());
    groupBoxUnit->setEnabled(flag);
}

void MsrUnitWidget::revert()
{
    m_mapperUnit->revert();
    edit(false);
}

void MsrUnitWidget::setRootIndex(const QModelIndex &index)
{
    QModelIndex rootIndex = m_unitModel->mapToSource(tableViewUnit->rootIndex());
    if (rootIndex == index)
        return;

    m_unitModel->setFilterIndex(index);
//    for (int row=0;row<comboBoxAttrGroup->count();row++)
//        comboBoxAttrGroup->removeItem(row);

//    for (int row=0;row<m_attrModel->rowCount(m_attrModel->mapFromSource(index));row++){
//        QString insText = m_attrModel->mapFromSource(index).child(
//                    row,m_model->columnDisplayedAttr(DBATTRXML::ATTR,
//                                                    DBATTRXML::GROUP)).data().toString();
//        if (comboBoxAttrGroup->findText(insText)==-1)
//            comboBoxAttrGroup->addItem(insText);
//    }

    m_unitModel->setSourceModel(m_model);
    tableViewUnit->setRootIndex(m_unitModel->mapFromSource(index));
    m_mapperUnit->setRootIndex(m_unitModel->mapFromSource(index));
}

QVariant MsrUnitWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}
