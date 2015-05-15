#include "permissionwidget.h"

#include <QMessageBox>

#include <metadatamodel/dbxmlstruct.h>

#include "permdelegate.h"


using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

PermissionWidget::PermissionWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    connect(toolButtonAdd,    SIGNAL(clicked()), this, SLOT(add()   ));
    connect(toolButtonDelete, SIGNAL(clicked()), this, SLOT(remove()));

    connect(checkBoxInInherited,SIGNAL(clicked(bool)),
            this,SLOT(showParent(bool)));

    connect(treeViewPerm, SIGNAL(clicked(QModelIndex)),
            this, SLOT(cellItemEdit(QModelIndex)));

    treeViewPerm->setItemDelegate(new PermDelegate(this));
    m_proxyModel = new PermissionProxyModel();
}

PermissionWidget::~PermissionWidget()
{
    delete m_proxyModel;
}

TreeXmlHashModel *PermissionWidget::model()
{
    return m_model;
}

PermissionProxyModel *PermissionWidget::proxyModel()
{
     return m_proxyModel;
}

void PermissionWidget::setModel(TreeXmlHashModel *model)
{
    m_model = model;
    m_proxyModel->setSourceModel(m_model);
    treeViewPerm->setModel(m_proxyModel);

    proxyModel()->setHeaderData(0,  Qt::Horizontal, tr("Наименование/Роль"));
    proxyModel()->setHeaderData(1,  Qt::Horizontal, tr("Созд."));
    proxyModel()->setHeaderData(2,  Qt::Horizontal, tr("Чтен."));
    proxyModel()->setHeaderData(3,  Qt::Horizontal, tr("Зап."));
    proxyModel()->setHeaderData(4,  Qt::Horizontal, tr("Удал."));
    proxyModel()->setHeaderData(5,  Qt::Horizontal, tr("Блок."));
    proxyModel()->setHeaderData(6,  Qt::Horizontal, tr("Объект"));
    proxyModel()->setHeaderData(7,  Qt::Horizontal, tr("Идентификатор"));
    treeViewPerm->setModel(proxyModel());

    for (int column = 6; column < 16; column++)
        treeViewPerm->setColumnHidden(column,true);

    treeViewPerm->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    treeViewPerm->header()->setDefaultSectionSize(50);
    treeViewPerm->header()->setDefaultAlignment(Qt::AlignCenter);
}

bool PermissionWidget::isRemove(const QModelIndex &proxyIndex)
{    
    if (proxyIndex.data(TreeXmlModel::TagRole) == DBPERMISSIONXML::PERMISSION)
        return true;

    return false;
}

void PermissionWidget::add()
{
    QString tag = DBPERMISSIONXML::PERMISSION;

    QModelIndex srcIndex = m_proxyModel->mapToSource(treeViewPerm->currentIndex());
    if (srcIndex.isValid()) {
        if (srcIndex.data(TreeXmlModel::TagRole) == tag) {
            srcIndex = srcIndex.parent();
            if (!srcIndex.isValid()) return;
        }
        QModelIndex srcCurrentIndex = m_model->insertLastRows(0,1,srcIndex, tag);
        if (srcCurrentIndex.isValid()){
            treeViewPerm->expand(m_proxyModel->mapFromSource(srcIndex));
            treeViewPerm->setCurrentIndex(m_proxyModel->mapFromSource(srcCurrentIndex));
            for (int column = 1; column <= 5; column++)
                m_model->setData(srcCurrentIndex.sibling(srcCurrentIndex.row(), column),
                                 false);

        }
    }
}

void PermissionWidget::remove()
{
    QPersistentModelIndex curIndex = treeViewPerm->currentIndex();
    QModelIndex srcIndex = curIndex.parent();

    if (srcIndex.isValid() && curIndex.isValid()){
        if (!isRemove(curIndex))
            return;
        treeViewPerm->setCurrentIndex(treeViewPerm->rootIndex());
        if (curIndex.isValid()) {
            m_proxyModel->removeRow(curIndex.row(),srcIndex);
            treeViewPerm->setModel(m_proxyModel);
        }
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить право доступа, поскольку нет выбраных значений."));
}

void PermissionWidget::setRootIndex(const QModelIndex &index)
{
    m_proxyModel->setRootIndex(index);
    QModelIndex proxyIndex = m_proxyModel->mapFromSource(index);
    treeViewPerm->setRootIndex(proxyIndex.parent());

    connect(treeViewPerm->selectionModel(),
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(currentIndexChange(QModelIndex,QModelIndex)));

    treeViewPerm->setCurrentIndex(proxyIndex);
    treeViewPerm->expand(proxyIndex);
}

void PermissionWidget::showParent(bool flag)
{
    proxyModel()->setFilterRole(Qt::EditRole);

    if (flag==true){
        proxyModel()->setFilterRegExp("");
    } else {
        QModelIndex index = treeViewPerm->rootIndex().child(0,0);
        QString className = modelData(DBCLASSXML::CLASS,
                                      DBCLASSXML::ID,
                                      index).toString();
        className.replace("{","\\{");
        className.replace("}","\\}");
        if (className.isEmpty()){
            proxyModel()->setFilterRegExp("\\S*");
        }else
            proxyModel()->setFilterRegExp(className);
    }
    proxyModel()->setFilterKeyColumn(
                model()->columnDisplayedAttr(DBATTRXML::ATTR, DBATTRXML::PARENT));
}

void PermissionWidget::currentIndexChange(const QModelIndex &current,
                                       const QModelIndex &previous)
{
    if (current.sibling(current.row(),0) == previous.sibling(previous.row(),0))
        return;

    QString currentTag = current.data(TreeXmlModel::TagRole).toString();
    toolButtonDelete->setDisabled(currentTag != DBPERMISSIONXML::PERMISSION);

    QString previousTag = previous.data(TreeXmlModel::TagRole).toString();
    QString previousRole = modelData(DBPERMISSIONXML::PERMISSION,
                             DBPERMISSIONXML::ROLE, previous).toString();
    if (previousRole.isEmpty() && previousTag == DBPERMISSIONXML::PERMISSION)
        proxyModel()->removeRow(previous.row(), previous.parent());
}

void PermissionWidget::cellItemEdit(const QModelIndex &index)
{
    if (index.column() != 0)
        treeViewPerm->edit(index);
}

QVariant PermissionWidget::modelData(const QString &tag, const QString &attr,
                                     const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}

}}
