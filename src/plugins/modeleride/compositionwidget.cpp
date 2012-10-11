#include "compositionwidget.h"
#include "dbxmlstruct.h"
#include <treexmlmodel/xmldelegate.h>
#include <QHeaderView>

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

    connect(tableViewComp,SIGNAL(clicked(QModelIndex)),
            this,SLOT(setCurrent(QModelIndex)));
    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDelete,SIGNAL(clicked()),this,SLOT(remove()));

    connect(toolButtonEdit,SIGNAL(clicked()),this,SLOT(edit()));
    connect(checkBoxInInherited,SIGNAL(clicked(bool)),this,SLOT(showParent(bool)));
}

CompositionWidget::~CompositionWidget()
{
    delete m_compositionModel;
}

void CompositionWidget::setModel(TreeXMLModel *model)
{
    m_model = model;

    m_compositionModel->setSourceModel(m_model);
    m_compositionModel->setHeaderData(0, Qt::Horizontal, tr("Имя состава"));
    m_compositionModel->setHeaderData(1, Qt::Horizontal, tr("Описание"));
    m_compositionModel->setHeaderData(2, Qt::Horizontal, tr("Исходный класс"));
    m_compositionModel->setHeaderData(3, Qt::Horizontal, tr("Входящий класс"));
    m_compositionModel->setHeaderData(4, Qt::Horizontal, tr("Описание включения"));
    m_compositionModel->setHeaderData(5, Qt::Horizontal, tr("Описание вхождения"));
    m_compositionModel->setColumnCount(6);
    tableViewComp->setModel(m_compositionModel);
}


void CompositionWidget::add()
{
}

void CompositionWidget::remove()
{
}

void CompositionWidget::setCurrent(QModelIndex index)
{
}

void CompositionWidget::edit()
{
}

QVariant CompositionWidget::modelData(QString typeName, QString attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->indexDisplayedAttr(
                             typeName,attr)).data();
}

void CompositionWidget::showParent(bool flag)
{
    m_compositionModel->setFilterRole(Qt::EditRole);

    if (flag==true){
        m_compositionModel->setFilterRegExp("");
    } else {
        QModelIndex index = tableViewComp->rootIndex();
        QString className = modelData(DBCLASSXML::CLASS,
                                      DBCLASSXML::NAME,
                                      index).toString();
        if (className.isEmpty()){
            m_compositionModel->setFilterRegExp("\\S*");
        }else
            m_compositionModel->setFilterRegExp(className);
    }
    m_compositionModel->setFilterKeyColumn(m_model->indexDisplayedAttr(DBCOMPXML::COMP,
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

    this->setCurrent(tableViewComp->rootIndex().child(0,0));
}
