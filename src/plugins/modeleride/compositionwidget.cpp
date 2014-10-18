#include "compositionwidget.h"
#include "dbxmlstruct.h"
#include "xmldelegate.h"
#include <QHeaderView>
#include <QMessageBox>

CompositionWidget::CompositionWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    m_compositionModel = new TableXMLProxyModel();
    QStringList tags;
    tags << DBCOMPXML::COMP;
    m_compositionModel->setAttributeTags(tags);

    tableViewComp->setItemDelegate(new XmlDelegate(this));
    tableViewComp->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDelete,SIGNAL(clicked()),this,SLOT(remove()));

    connect(toolButtonEdit,SIGNAL(clicked()),this,SLOT(edit()));
    connect(checkBoxInInherited,SIGNAL(clicked(bool)),this,SLOT(showParent(bool)));

    connect(toolButtonDown,SIGNAL(clicked()),this,SLOT(down()));
    connect(toolButtonUp,SIGNAL(clicked()),this,SLOT(up()));
}

CompositionWidget::~CompositionWidget()
{
    delete m_compositionModel;
}

void CompositionWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;

    m_compositionModel->setSourceModel(m_model);
    m_compositionModel->setHeaderData(0, Qt::Horizontal, tr("Имя состава"));
    m_compositionModel->setHeaderData(1, Qt::Horizontal, tr("Псевдоним"));
    m_compositionModel->setHeaderData(2, Qt::Horizontal, tr("Исходный класс"));
    m_compositionModel->setHeaderData(3, Qt::Horizontal, tr("Входящий класс"));
    m_compositionModel->setHeaderData(4, Qt::Horizontal, tr("Представление"));
    m_compositionModel->setHeaderData(5, Qt::Horizontal, tr("Композиция"));
    m_compositionModel->setHeaderData(6, Qt::Horizontal, tr("Описание включения"));
    m_compositionModel->setHeaderData(7, Qt::Horizontal, tr("Описание вхождения"));
    m_compositionModel->setHeaderData(8, Qt::Horizontal, tr("Индетификатор"));
    m_compositionModel->setColumnCount(9);
    m_compositionModel->setDynamicSortFilter(true);

    tableViewComp->setModel(m_compositionModel);
    tableViewComp->setColumnHidden(8,true);
}


void CompositionWidget::add()
{
    QModelIndex srcIndex = m_compositionModel->mapToSource(tableViewComp->rootIndex());
    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex, DBCOMPXML::COMP);
    if (srcCurrentIndex.isValid()){
        tableViewComp->setCurrentIndex(
                    m_compositionModel->mapFromSource(srcCurrentIndex));
        emit dataEdited(srcCurrentIndex);
    }

}

void CompositionWidget::remove()
{   
    QModelIndex srcIndex = m_compositionModel->mapToSource(tableViewComp->rootIndex());
    QModelIndex curIndex = m_compositionModel->mapToSource(tableViewComp->currentIndex());
    if (srcIndex.isValid() && curIndex.isValid()){
        emit dataRemoved(srcIndex);
        m_model->removeRow(curIndex.row(),srcIndex);
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить состав, поскольку ничего не выбрано."));
}

void CompositionWidget::edit()
{
    QModelIndex index = m_compositionModel->mapToSource(tableViewComp->currentIndex());
    emit dataEdited(index);
}

void CompositionWidget::up()
{
    QPersistentModelIndex index = tableViewComp->currentIndex();
    QPersistentModelIndex srcIndex  = m_compositionModel->mapToSource(index);
    QPersistentModelIndex srcParent = srcIndex.parent();
    int row = m_compositionModel->mapToSource(index.sibling(index.row()-1,0)).row();
    if (m_model->moveIndex(srcIndex,srcParent,row)) {
        if (row >= 0)
            index = tableViewComp->currentIndex().sibling(index.row()-2,0);
        else
            index = index.sibling(0,0);
        m_model->removeRow(srcIndex.row(),srcIndex.parent());
        tableViewComp->setCurrentIndex(index);
    }
}

void CompositionWidget::down()
{
    QPersistentModelIndex index = tableViewComp->currentIndex();
    QPersistentModelIndex srcIndex  = m_compositionModel->mapToSource(index);
    QPersistentModelIndex srcParent = srcIndex.parent();
    QModelIndex indexNew = index.sibling(index.row()+2,0);
    int row = (indexNew.isValid()) ? m_compositionModel->mapToSource(indexNew).row()
                                   : m_model->rowCount(srcParent,
                                                       m_model->tagsFilter(),
                                                       QStringList());
    if (m_model->moveIndex(srcIndex,srcParent,row)) {
        index = tableViewComp->currentIndex().sibling(index.row()+2,0);
        m_model->removeRow(srcIndex.row(),srcIndex.parent());
        tableViewComp->setCurrentIndex(index);
    }
}

QVariant CompositionWidget::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}

void CompositionWidget::showParent(bool flag)
{
    m_compositionModel->setFilterRole(Qt::EditRole);

    if (flag==true){
        m_compositionModel->setFilterRegExp("");
    } else {
        QModelIndex index = tableViewComp->rootIndex();
        QString className = modelData(DBCLASSXML::CLASS,
                                      DBCLASSXML::ID,
                                      index).toString();
        className.replace("{","\\{");
        className.replace("}","\\}");
        if (className.isEmpty()){
            m_compositionModel->setFilterRegExp("\\S*");
        }else
            m_compositionModel->setFilterRegExp(className);
    }
    m_compositionModel->setFilterKeyColumn(m_model->columnDisplayedAttr(DBCOMPXML::COMP,
                                                                DBCOMPXML::PARENT));
}

void CompositionWidget::setRootIndex(QModelIndex index)
{
    QModelIndex rootIndex = m_compositionModel->mapToSource(tableViewComp->rootIndex());
    if (rootIndex == index)
        return;

    m_compositionModel->setFilterIndex(index);
    m_compositionModel->setSourceModel(m_model);

    tableViewComp->setRootIndex(m_compositionModel->mapFromSource(index));
    tableViewComp->setCurrentIndex(tableViewComp->rootIndex().child(0,0));
}
