#include "permissionwidget.h"

#include <QMessageBox>

#include <metadatamodel/dbxmlstruct.h>

#include "permdelegate.h"

using namespace RTPTechGroup::XmlModel;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

PermissionWidget::PermissionWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    connect(toolButtonAdd, &QToolButton::clicked,
            this, &PermissionWidget::add);
    connect(toolButtonDelete, &QToolButton::clicked,
            this, &PermissionWidget::remove);

    connect(checkBoxInInherited, &QCheckBox::clicked,
            this, &PermissionWidget::showParent);

    connect(treeViewPerm, &QTreeView::clicked,
            this, &PermissionWidget::cellItemEdit);

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

    for (qint32 column = 6; column < 18; ++column)
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
            for (qint32 column = 1; column <= 5; ++column)
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
        QMessageBox::warning(
            nullptr, tr("Предупреждение"),
            tr("Невозможно удалить право доступа, поскольку нет выбранных значений.")
        );
}

void PermissionWidget::setRootIndex(const QModelIndex &index)
{
    m_proxyModel->setRootIndex(index);
    QModelIndex proxyIndex = m_proxyModel->mapFromSource(index);
    treeViewPerm->setRootIndex(proxyIndex.parent());

    connect(treeViewPerm->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &PermissionWidget::currentIndexChange);

    treeViewPerm->setCurrentIndex(proxyIndex);
    treeViewPerm->expand(proxyIndex);
}

void PermissionWidget::showParent(bool flag)
{
    proxyModel()->setFilterRole(Qt::EditRole);

    if (flag==true){
        proxyModel()->setFilterRegExp("");
    } else {
        QModelIndex index = childIdx(0,0,treeViewPerm->rootIndex());
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

QModelIndex PermissionWidget::childIdx(int arow, int acolumn, const QModelIndex &parent) const
{
    return parent.model() ? parent.model()->index(arow, acolumn, parent)
                          : QModelIndex();
}

QVariant PermissionWidget::modelData(const QString &tag, const QString &attr,
                                     const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                             tag,attr)).data();
}

}}
