#include "permdelegate.h"

#include <QApplication>
#include <QComboBox>
#include <QCheckBox>

#include <QAbstractProxyModel>

#include <treexmlmodel/treexmlmodel.h>
#include <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::XmlModel;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

PermDelegate::PermDelegate(QObject *parent)
    :XmlDelegate(parent)
{

}

PermDelegate::~PermDelegate()
{

}

QWidget *PermDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    QString tag  = index.data(TreeXmlModel::TagRole).toString();
    if (tag != DBPERMISSIONXML::PERMISSION)
        return NULL;

    const TreeXmlHashModel* hashModel =this->hashModel(index.model());
    if (hashModel) {
        QString attr = hashModel->displayedAttr(tag, index.column());
        if (tag == DBPERMISSIONXML::PERMISSION && attr == DBPERMISSIONXML::ROLE) {
            QComboBox *comboBox = new QComboBox(parent);
            comboBox->setItemDelegate(new XmlDelegate(comboBox));
            comboBox->setModel(const_cast<TreeXmlHashModel* >(hashModel));
            comboBox->setRootModelIndex(
                        hashModel->index(0,0).child(4,0));
            return comboBox;
        }
    }

    QWidget *widget = XmlDelegate::createEditor(parent, option, index);
    QCheckBox *checkBox = qobject_cast<QCheckBox *>(widget);
    if (checkBox && index.data(Qt::EditRole).type() == QVariant::Bool) {
        QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
        model->setData(index,true);
    }

    return widget;
}

void PermDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    XmlDelegate::setEditorData(editor,index);
}

void PermDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    if (index.data(TreeXmlHashModel::TagRole) != DBPERMISSIONXML::PERMISSION
            && index.column() != 0)
    {
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &option, painter);
        return;
    }
    XmlDelegate::paint(painter, option, index);
}

const TreeXmlHashModel *PermDelegate::hashModel(const QAbstractItemModel *model) const
{
    const QAbstractProxyModel *proxyModel = dynamic_cast<const QAbstractProxyModel *>(model);
    const TreeXmlHashModel* hashModel = (proxyModel)?
                dynamic_cast<const TreeXmlHashModel*>(proxyModel->sourceModel())
              : dynamic_cast<const TreeXmlHashModel*>(model);
    return hashModel;
}

}}

