#include "refitemdelegate.h"

#include <QSortFilterProxyModel>
#include <QLineEdit>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <guiextension/treecombobox.h>
#include <metadatamodel/dbxmlstruct.h>

#include "refitemproxymodel.h"

using namespace RTPTechGroup::Widgets;
using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

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
    const TreeXmlHashModel *hashModel = this->hashModel(index.model());

    if (hashModel) {
        QString attr = hashModel->displayedAttr(tag, index.column());

        if (tag == DBLINKTOFILTERXML::LINKTOFILTER
             && attr == DBLINKTOFILTERXML::REFFILTER)
        {
            TreeComboBox *comboBoxDestClass = new TreeComboBox(parent);
            comboBoxDestClass->setItemDelegate(new XmlDelegate(comboBoxDestClass));
            TableXMLProxyModel *classFilterModel = new TableXMLProxyModel(parent);

            QStringList tags;
            tags << DBFILTERXML::FILTER;

            classFilterModel->setFilterAllRows(true);
            classFilterModel->setAttributeTags(tags);
            classFilterModel->setFilterKeyColumn(0);
            classFilterModel->setFilterRole(TreeXmlModel::TagRole);
            classFilterModel->setFilterRegExp(DBFILTERXML::FILTER + "|" +
                                              DBCLASSXML::CLASS + "|" +
                                              DBMODELXML::MODEL + "|" +
                                              DBCLASSLISTXML::CLASSLIST);

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

            if (srcParentIndex.isValid()) {
                QModelIndex parentIndex = classFilterModel->mapFromSource(srcParentIndex);
                comboBoxDestClass->setRootModelIndex(parentIndex);
                comboBoxDestClass->setCurrentModelIndex(QModelIndex());
            } else {
                comboBoxDestClass->setRootModelIndex(
                            childIdx(0,0,classFilterModel->index(0,0)));

                comboBoxDestClass->setCurrentModelIndex(
                            childIdx(0,0,childIdx(0,0,classFilterModel->index(0,0))));
            }
            return comboBoxDestClass;
        } else if (tag == DBLINKTOCLASSXML::LINKTOCLASS
                    && attr == DBLINKTOCLASSXML::REFCLASS) {
            TreeComboBox *comboBoxDestClass = new TreeComboBox(parent);
            comboBoxDestClass->setItemDelegate(new XmlDelegate(comboBoxDestClass));

            QModelIndex srcParentIndex = rootClass(index);
            TableXMLProxyModel *classFilterModel = new TableXMLProxyModel(parent);
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
                classFilterModel->setFilterIndex(childIdx(0,0,hashModel->index(0,0)));
            classFilterModel->setDynamicSortFilter(true);
            classFilterModel->sort(0);

            comboBoxDestClass->setModel(classFilterModel);
            comboBoxDestClass->setIndexColumn(
                        hashModel->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                       DBCLASSXML::ID));

            if (srcParentIndex.isValid()) {
                QModelIndex parentIndex = classFilterModel->mapFromSource(srcParentIndex);
                comboBoxDestClass->setRootModelIndex(parentIndex.parent());
                comboBoxDestClass->setCurrentModelIndex(QModelIndex());
            } else {
                comboBoxDestClass->setRootModelIndex(
                            childIdx(0,0,classFilterModel->index(0,0)));

                comboBoxDestClass->setCurrentModelIndex(
                            childIdx(0,0,childIdx(0,0,classFilterModel->index(0,0))));
            }
            return comboBoxDestClass;
        } else  if (tag == DBLINKTOREFXML::LINKTOREF
                    && attr == DBLINKTOREFXML::REFREF) {
            TreeComboBox *comboBoxDestClass = new TreeComboBox(parent);
            comboBoxDestClass->setItemDelegate(new XmlDelegate(comboBoxDestClass));


            RefItemProxyModel *classFilterModel = new RefItemProxyModel(parent);

            classFilterModel->setFilterKeyColumn(0);
            classFilterModel->setFilterRole(TreeXmlModel::TagRole);
            classFilterModel->setFilterRegExp(DBREFGROUPXML::REFGROUP + "|" +
                                              DBREFXML::REF + "|" +
                                              DBMODELXML::MODEL + "|" +
                                              DBREFLISTXML::REFLIST);

            classFilterModel->setSourceModel(
                        const_cast<QAbstractItemModel *>(
                            dynamic_cast<const QAbstractItemModel*>(hashModel)));

            classFilterModel->setDynamicSortFilter(true);
            classFilterModel->sort(0);

            QModelIndex srcParentIndex = rootClass(index);
            classFilterModel->setClassIndex(srcParentIndex);

            comboBoxDestClass->setModel(classFilterModel);
            comboBoxDestClass->setIndexColumn(
                        hashModel->columnDisplayedAttr(DBREFXML::REF,
                                                       DBREFXML::ID));

            comboBoxDestClass->setRootModelIndex(
                        childIdx(0,0,classFilterModel->index(0,0)));

            comboBoxDestClass->setCurrentModelIndex(
                        childIdx(0,0,childIdx(0,0,classFilterModel->index(0,0))));

            return comboBoxDestClass;
        } else
            return new QLineEdit(parent);
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void RefItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    TreeComboBox *treeComboBox = dynamic_cast<TreeComboBox*>(editor);
    if (treeComboBox) {
        // Проверка вводимых данных
        QModelIndex curIndex = treeComboBox->currentModelIndex();
        if (curIndex.isValid()) {
            QString tag = curIndex.data(TreeXmlModel::TagRole).toString();
            if (tag != DBFILTERXML::FILTER && tag != DBCLASSXML::CLASS
                    && tag != DBREFXML::REF)
                return;
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
        const TreeXmlHashModel *hashModel = this->hashModel(index.model());
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
            } else if (parentIndex.data(TreeXmlModel::TagRole) == DBLINKTOCLASSXML::LINKTOCLASS) {
                QModelIndex linkIndex = parentIndex.sibling(
                            parentIndex.row(),
                            hashModel->columnDisplayedAttr(DBLINKTOCLASSXML::LINKTOCLASS,
                                                           DBLINKTOCLASSXML::REFCLASS));
                QModelIndex classIndex = hashModel->indexHashAttr(DBCLASSXML::CLASS,
                                                                  DBCLASSXML::ID,
                                                                  linkIndex.data(Qt::EditRole));
                return classIndex;
            } else if (parentIndex.data(TreeXmlModel::TagRole) == DBLINKTOREFXML::LINKTOREF) {
                QModelIndex linkIndex = parentIndex.sibling(
                            parentIndex.row(),
                            hashModel->columnDisplayedAttr(DBLINKTOREFXML::LINKTOREF,
                                                           DBLINKTOREFXML::REFREF));
                QModelIndex classIndex = hashModel->indexHashAttr(DBLINKTOREFXML::LINKTOREF,
                                                                  DBREFXML::ID,
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
    const TreeXmlHashModel *hashModel = (proxyModel)?
                dynamic_cast<const TreeXmlHashModel*>(proxyModel->sourceModel())
              : dynamic_cast<const TreeXmlHashModel*>(model);
    return hashModel;
}

}}
