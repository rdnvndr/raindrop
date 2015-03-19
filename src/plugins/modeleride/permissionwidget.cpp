#include "permissionwidget.h"

#include <QMessageBox>

#include <metadatamodel/dbxmlstruct.h>

#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

PermissionWidget::PermissionWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    connect(toolButtonAdd,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDelete,SIGNAL(clicked()),this,SLOT(remove()));

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
    proxyModel()->setHeaderData(1,  Qt::Horizontal, tr("Создание"));
    proxyModel()->setHeaderData(2,  Qt::Horizontal, tr("Чтение"));
    proxyModel()->setHeaderData(3,  Qt::Horizontal, tr("Запись"));
    proxyModel()->setHeaderData(4,  Qt::Horizontal, tr("Удаление"));
    proxyModel()->setHeaderData(5,  Qt::Horizontal, tr("Блокировка"));
    proxyModel()->setHeaderData(6,  Qt::Horizontal, tr("Объект"));
    proxyModel()->setHeaderData(7,  Qt::Horizontal, tr("Идентификатор"));
    treeViewPerm->setModel(proxyModel());

    for (int column = 6; column < 16; column++)
        treeViewPerm->setColumnHidden(column,true);
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
    QModelIndex srcCurrentIndex =
            m_model->insertLastRows(0,1,srcIndex, tag);
    if (srcCurrentIndex.isValid()){
        edit(true);
    }
}

void PermissionWidget::remove()
{
    QModelIndex curIndex = treeViewPerm->currentIndex();
    QModelIndex srcIndex = curIndex.parent();

    if (srcIndex.isValid() && curIndex.isValid()){
        if (!isRemove(curIndex))
            return;
        treeViewPerm->setCurrentIndex(treeViewPerm->rootIndex());
        m_proxyModel->removeRow(curIndex.row(),srcIndex);
        treeViewPerm->setModel(m_proxyModel);
    } else
        QMessageBox::warning(NULL,tr("Предупреждение"),
                             tr("Невозможно удалить значение списка, поскольку нет выбраных значений."));
}

void PermissionWidget::edit(bool flag)
{
    if (flag == false)
        treeViewPerm->setCurrentIndex(treeViewPerm->rootIndex());

    toolButtonAdd->setEnabled(flag);
    toolButtonDelete->setEnabled(flag);
}

void PermissionWidget::setRootIndex(const QModelIndex &index)
{
    QModelIndex rootIndex = m_proxyModel->mapToSource(treeViewPerm->rootIndex());
    if (rootIndex == index)
        return;

    m_proxyModel->setRootIndex(index);
    treeViewPerm->setRootIndex(m_proxyModel->mapFromSource(index).parent());
    treeViewPerm->expandAll();

    emit proxyIndexChanged(treeViewPerm->rootIndex());
}

}}
