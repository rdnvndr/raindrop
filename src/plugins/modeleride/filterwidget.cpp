#include "filterwidget.h"
#include "dbxmlstruct.h"
#include "xmldelegate.h"
#include <QHeaderView>
#include <QMessageBox>

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_filterModel = new TableXMLProxyModel();
    QStringList tags;
    tags << DBFILTERXML::FILTER;
    m_filterModel->setAttributeTags(tags);

    tableViewFilter->setItemDelegate(new XmlDelegate(this));
    tableViewFilter->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDelete,SIGNAL(clicked()),this,SLOT(remove()));

    connect(toolButtonEdit,SIGNAL(clicked()),this,SLOT(edit()));
    connect(checkBoxInInherited,SIGNAL(clicked(bool)),this,SLOT(showParent(bool)));
}

FilterWidget::~FilterWidget()
{
    delete m_filterModel;
}

void FilterWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    m_filterModel->setSourceModel(m_model);
    m_filterModel->setHeaderData(0, Qt::Horizontal, tr("Имя фильтра"));
    m_filterModel->setHeaderData(1, Qt::Horizontal, tr("Описание"));
    m_filterModel->setHeaderData(2, Qt::Horizontal, tr("Первый класс"));
    m_filterModel->setHeaderData(3, Qt::Horizontal, tr("Второй класс"));
    m_filterModel->setHeaderData(4, Qt::Horizontal, tr("Описание прямое"));
    m_filterModel->setHeaderData(5, Qt::Horizontal, tr("Описание обратное"));
    m_filterModel->setHeaderData(6, Qt::Horizontal, tr("Индетификатор"));
    m_filterModel->setColumnCount(7);
    m_filterModel->setDynamicSortFilter(true);

    tableViewFilter->setSortingEnabled(true);
    tableViewFilter->setModel(m_filterModel);
    tableViewFilter->sortByColumn(0,Qt::AscendingOrder);
    tableViewFilter->setColumnHidden(6,true);
}

void FilterWidget::add()
{
    QModelIndex srcIndex = m_filterModel->mapToSource(tableViewFilter->rootIndex());
    m_model->setInsTagName(DBFILTERXML::FILTER);
    if (m_model->insertRow(0,srcIndex)){
        QModelIndex srcCurrentIndex = m_model->lastInsertRow();
        tableViewFilter->setCurrentIndex(
                    m_filterModel->mapFromSource(srcCurrentIndex));
        emit dataEdited(srcCurrentIndex);
    }
}

void FilterWidget::remove()
{
    QModelIndex srcIndex = m_filterModel->mapToSource(tableViewFilter->rootIndex());
    QModelIndex curIndex = m_filterModel->mapToSource(tableViewFilter->currentIndex());
    if (srcIndex.isValid() && curIndex.isValid()){
        emit dataRemoved(srcIndex);
        m_model->removeRow(curIndex.row(),srcIndex);
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить состав, поскольку ничего не выбрано."));

}

void FilterWidget::edit()
{
    QModelIndex index = m_filterModel->mapToSource(tableViewFilter->currentIndex());
    emit dataEdited(index);
}

void FilterWidget::showParent(bool flag)
{
    m_filterModel->setFilterRole(Qt::EditRole);

    if (flag==true){
        m_filterModel->setFilterRegExp("");
    } else {
        QModelIndex index = tableViewFilter->rootIndex();
        QString className = modelData(DBCLASSXML::CLASS,
                                      DBCLASSXML::ID,
                                      index).toString();
        className.replace("{","\\{");
        className.replace("}","\\}");
        if (className.isEmpty()){
            m_filterModel->setFilterRegExp("\\S*");
        }else
            m_filterModel->setFilterRegExp(className);
    }
    m_filterModel->setFilterKeyColumn(m_model->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                                DBFILTERXML::PARENT));
}

void FilterWidget::setRootIndex(QModelIndex index)
{
    QModelIndex rootIndex = m_filterModel->mapToSource(tableViewFilter->rootIndex());
    if (rootIndex == index)
        return;

    m_filterModel->setFilterIndex(index);
    m_filterModel->setSourceModel(m_model);

    tableViewFilter->setRootIndex(m_filterModel->mapFromSource(index));
    tableViewFilter->setCurrentIndex(tableViewFilter->rootIndex().child(0,0));
}

QVariant FilterWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}
