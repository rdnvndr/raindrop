#include "refitemdelegate.h"

#include <QSortFilterProxyModel>
#include <QLineEdit>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <treecombobox/treecombobox.h>
#include <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::Widgets;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

RefItemDelegate::RefItemDelegate(QObject *parent) :
    XmlDelegate(parent)
{
}

QWidget *RefItemDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem
                                       &option, const QModelIndex &index) const
{
    QString tag  = index.data(TreeXmlModel::TagRole).toString();
    const TreeXmlHashModel* hashModel = this->hashModel(index.model());

    if (hashModel) {
        QString attr = hashModel->displayedAttr(tag, index.column());

        if ((tag == DBLINKTOFILTERXML::LINKTOFILTER
             && attr == DBLINKTOFILTERXML::REFFILTER)
                || (tag == DBLINKTOCOMPXML::LINKTOCOMP
                    && attr == DBLINKTOCOMPXML::REFCOMP))
        {
            TreeComboBox *comboBoxDestClass = new TreeComboBox(parent);
            comboBoxDestClass->setItemDelegate(new XmlDelegate());
            TableXMLProxyModel* classFilterModel = new TableXMLProxyModel(parent);
            QStringList tags;
            tags << DBFILTERXML::FILTER << DBCOMPXML::COMP;
            classFilterModel->setAttributeTags(tags);
            classFilterModel->setSourceModel(const_cast<QAbstractItemModel *>(
                                                 dynamic_cast<const QAbstractItemModel*>(hashModel)));

            QModelIndex srcParentIndex = rootClass(index);
            classFilterModel->setFilterIndex(srcParentIndex);

            classFilterModel->setDynamicSortFilter(true);
            classFilterModel->sort(0);

            comboBoxDestClass->setModel(classFilterModel);


            if (tag == DBLINKTOFILTERXML::LINKTOFILTER)
                comboBoxDestClass->setIndexColumn(
                            hashModel->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                           DBFILTERXML::ID));
            else
                comboBoxDestClass->setIndexColumn(
                            hashModel->columnDisplayedAttr(DBCOMPXML::COMP,
                                                           DBCOMPXML::ID));

            if (srcParentIndex.isValid()) {
                QModelIndex parentIndex = classFilterModel->mapFromSource(srcParentIndex);
                comboBoxDestClass->setRootModelIndex(parentIndex);
                comboBoxDestClass->setCurrentModelIndex(QModelIndex());
            } else {
                comboBoxDestClass->setRootModelIndex(
                            classFilterModel->index(0,0).child(0,0));

                comboBoxDestClass->setCurrentModelIndex(
                            classFilterModel->index(0,0).child(0,0).child(0,0));
            }
            return comboBoxDestClass;
        } else if (tag == DBLINKTOCLASSXML::LINKTOCLASS
                    && attr == DBLINKTOCLASSXML::REFCLASS) {
            TreeComboBox *comboBoxDestClass = new TreeComboBox(parent);
            comboBoxDestClass->setItemDelegate(new XmlDelegate());

            QModelIndex srcParentIndex = rootClass(index);
            TableXMLProxyModel* classFilterModel = new TableXMLProxyModel(parent);
            QStringList tags;
            tags << DBCLASSXML::CLASS;
            classFilterModel->setFilterAllRows(true);
            classFilterModel->setAttributeTags(tags);
            classFilterModel->setFilterKeyColumn(0);
            classFilterModel->setFilterRole(TreeXmlModel::TagRole);
            classFilterModel->setFilterRegExp(DBCLASSXML::CLASS + "|" +
                                              DBMODELXML::MODEL + "|" +
                                              DBCLASSLISTXML::CLASSLIST);

            classFilterModel->setSourceModel(const_cast<QAbstractItemModel *>(
                                                 dynamic_cast<const QAbstractItemModel*>(hashModel)));
            if (srcParentIndex.isValid())
                classFilterModel->setFilterIndex(srcParentIndex);
            else
                classFilterModel->setFilterIndex(hashModel->index(0,0).child(0,0));
            classFilterModel->setDynamicSortFilter(true);
            classFilterModel->sort(0);

            comboBoxDestClass->setModel(classFilterModel);
            comboBoxDestClass->setIndexColumn(
                        hashModel->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                       DBATTRXML::ID));

            if (srcParentIndex.isValid()) {
                QModelIndex parentIndex = classFilterModel->mapFromSource(srcParentIndex);
                comboBoxDestClass->setRootModelIndex(parentIndex.parent());
                comboBoxDestClass->setCurrentModelIndex(QModelIndex());
            } else {
                comboBoxDestClass->setRootModelIndex(
                            classFilterModel->index(0,0).child(0,0));

                comboBoxDestClass->setCurrentModelIndex(
                            classFilterModel->index(0,0).child(0,0).child(0,0));
            }
            return comboBoxDestClass;
        } else
            return new QLineEdit(parent);
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void RefItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    TreeComboBox* treeComboBox = dynamic_cast<TreeComboBox*>(editor);
    if (treeComboBox) {
        QModelIndex curIndex = treeComboBox->currentModelIndex();
        if (curIndex.isValid()) {
            QString tag = curIndex.data(TreeXmlModel::TagRole).toString();
            QAbstractItemModel *indexModel = model;//const_cast<QAbstractItemModel *>(index.model());
            const TreeXmlHashModel *hashModel = this->hashModel(model);
            if (tag == DBCOMPXML::COMP) {
                treeComboBox->setIndexColumn(hashModel->columnDisplayedAttr(DBCOMPXML::COMP,
                                                                            DBCOMPXML::ID));
                indexModel->setData(index, DBLINKTOCOMPXML::LINKTOCOMP, TreeXmlModel::TagRole);
            } else if (tag == DBFILTERXML::FILTER) {
                treeComboBox->setIndexColumn(hashModel->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                                            DBFILTERXML::ID));
                indexModel->setData(index, DBLINKTOFILTERXML::LINKTOFILTER, TreeXmlModel::TagRole);
            }
        }
    }
    XmlDelegate::setModelData(editor, model, index);
}


void RefItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    QStyledItemDelegate::setEditorData(editor,index);
}

QModelIndex RefItemDelegate::rootClass(QModelIndex index) const
{
    if (index.isValid()) {
        const TreeXmlHashModel* hashModel = this->hashModel(index.model());
        if (hashModel) {
            QModelIndex parentIndex = index.parent();
            if (parentIndex.data(TreeXmlModel::TagRole) == DBLINKTOFILTERXML::LINKTOFILTER) {
                QModelIndex linkIndex = parentIndex.sibling(
                            parentIndex.row(),
                            hashModel->columnDisplayedAttr(DBLINKTOFILTERXML::LINKTOFILTER,
                                                           DBLINKTOFILTERXML::REFFILTER));
                QModelIndex filterIndex = hashModel->indexHashAttr(DBFILTERXML::FILTER,
                                                                   DBFILTERXML::ID,
                                                                   linkIndex.data(Qt::EditRole));
                linkIndex = filterIndex.sibling(filterIndex.row(),
                                                hashModel->columnDisplayedAttr(
                                                    DBFILTERXML::FILTER,
                                                    DBFILTERXML::CLASS));
                QModelIndex classIndex = hashModel->indexLink(linkIndex);
                return classIndex;
            } else  if (parentIndex.data(TreeXmlModel::TagRole) == DBLINKTOCOMPXML::LINKTOCOMP) {
                QModelIndex linkIndex = parentIndex.sibling(
                            parentIndex.row(),
                            hashModel->columnDisplayedAttr(DBLINKTOCOMPXML::LINKTOCOMP,
                                                           DBLINKTOCOMPXML::REFCOMP));
                QModelIndex filterIndex = hashModel->indexHashAttr(DBCOMPXML::COMP,
                                                                   DBCOMPXML::ID,
                                                                   linkIndex.data(Qt::EditRole));
                linkIndex = filterIndex.sibling(filterIndex.row(),
                                                hashModel->columnDisplayedAttr(
                                                    DBCOMPXML::COMP,
                                                    DBCOMPXML::CLASS));
                QModelIndex classIndex = hashModel->indexLink(linkIndex);
                return classIndex;
            } else if (parentIndex.data(TreeXmlModel::TagRole) == DBLINKTOCLASSXML::LINKTOCLASS) {
                QModelIndex linkIndex = parentIndex.sibling(
                            parentIndex.row(),
                            hashModel->columnDisplayedAttr(DBLINKTOCLASSXML::LINKTOCLASS,
                                                           DBLINKTOCLASSXML::REFCLASS));
                QModelIndex classIndex = hashModel->indexHashAttr(DBCLASSXML::CLASS,
                                                                  DBCLASSXML::ID,
                                                                  linkIndex.data(Qt::EditRole));
                return classIndex;
            }
        }
    }
    return QModelIndex();
}

const TreeXmlHashModel *RefItemDelegate::hashModel(const QAbstractItemModel *model) const
{
    const QAbstractProxyModel *proxyModel = dynamic_cast<const QAbstractProxyModel *>(model);
    const TreeXmlHashModel* hashModel = (proxyModel)?
                dynamic_cast<const TreeXmlHashModel*>(proxyModel->sourceModel())
              : dynamic_cast<const TreeXmlHashModel*>(model);
    return hashModel;
}

}}
