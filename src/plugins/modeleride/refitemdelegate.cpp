#include "refitemdelegate.h"

#include <QSortFilterProxyModel>
#include <QLineEdit>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>
#include <treecombobox/treecombobox.h>
#include "dbxmlstruct.h"

RefItemDelegate::RefItemDelegate(QObject *parent) :
    XmlDelegate(parent)
{
}

void RefItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
//    TreeComboBox* comboBoxDestClass = dynamic_cast<TreeComboBox*>(editor);
//    if (comboBoxDestClass) {
//        ModifyProxyModel* modifyModel = dynamic_cast<ModifyProxyModel*>(model);
//        TreeXmlHashModel* hashModel = (modifyModel)?
//                    dynamic_cast<TreeXmlHashModel*>(modifyModel->sourceModel())
//                  : dynamic_cast<TreeXmlHashModel*>(model);

//        if (hashModel) {
//            QString valueTag = comboBoxDestClass->currentData(
//                        TreeXmlModel::TagRole).toString();
//            QString tag  = index.data(TreeXmlModel::TagRole).toString();
//            QString attr = hashModel->displayedAttr(tag, index.column());
//            QString data = comboBoxDestClass->currentModelIndex().data().toString();
//            if (tag == DBLINKTOFILTERXML::LINKTOFILTER
//                    && attr == DBLINKTOFILTERXML::REFFILTER)
//            {
//                if (valueTag == DBFILTERXML::FILTER)
//                    model->setData(index,
//                                   data,
//                                   Qt::EditRole);
//                return;

//            }

//            if (tag == DBLINKTOCLASSXML::LINKTOCLASS
//                    && attr == DBLINKTOCLASSXML::REFCLASS)
//            {
//                if (valueTag == DBCLASSXML::CLASS)
//                    model->setData(index,
//                                   data,
//                                   Qt::EditRole);
//                return;
//            }
//        }
//    }

    XmlDelegate::setModelData(editor,model,index);
}

QWidget *RefItemDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem
                                       &option, const QModelIndex &index) const
{
    QString tag  = index.data(TreeXmlModel::TagRole).toString();

    const ModifyProxyModel *modifyModel = dynamic_cast<const ModifyProxyModel *>(index.model());
    const TreeXmlHashModel* hashModel = (modifyModel)?
                dynamic_cast<const TreeXmlHashModel*>(modifyModel->sourceModel())
              : dynamic_cast<const TreeXmlHashModel*>(index.model());

    if (hashModel) {
        QString attr = hashModel->displayedAttr(tag, index.column());
        if ((tag == DBLINKTOFILTERXML::LINKTOFILTER)
                && attr == DBLINKTOFILTERXML::REFFILTER
                || (tag == DBLINKTOCLASSXML::LINKTOCLASS
                    && attr == DBLINKTOCLASSXML::REFCLASS))
        {
            TreeComboBox *comboBoxDestClass = new TreeComboBox(parent);
            QSortFilterProxyModel* classFilterModel = new QSortFilterProxyModel(parent);
            classFilterModel->setFilterKeyColumn(0);
            classFilterModel->setFilterRole(TreeXmlModel::TagRole);
            classFilterModel->setSourceModel(
                        const_cast<QAbstractItemModel *>(
                            dynamic_cast<const QAbstractItemModel*>(hashModel)));
            if (tag == DBLINKTOFILTERXML::LINKTOFILTER) {
                classFilterModel->setFilterRegExp(
                            DBFILTERXML::FILTER + "|" +
                            DBCLASSXML::CLASS + "|" +
                            DBMODELXML::MODEL + "|" +
                            DBCLASSLISTXML::CLASSLIST);
            }  else {
                classFilterModel->setFilterRegExp(
                            DBCLASSXML::CLASS + "|" +
                            DBMODELXML::MODEL + "|" +
                            DBCLASSLISTXML::CLASSLIST);
            }
            classFilterModel->setDynamicSortFilter(true);
            classFilterModel->sort(0);

            comboBoxDestClass->setModel(classFilterModel);

            QModelIndex srcParentIndex = rootClass(index);
            comboBoxDestClass->setIndexColumn(
                        hashModel->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                       DBATTRXML::ID));
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
        } else
             return new QLineEdit(parent);
    }

    return XmlDelegate::createEditor(parent, option, index);
}

void RefItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    XmlDelegate::setEditorData(editor, index);
}

QModelIndex RefItemDelegate::rootClass(QModelIndex index) const
{
    if (index.isValid()) {
        const ModifyProxyModel *modifyModel = dynamic_cast<const ModifyProxyModel *>(index.model());
        const TreeXmlHashModel* hashModel = (modifyModel)?
                    dynamic_cast<const TreeXmlHashModel*>(modifyModel->sourceModel())
                  : dynamic_cast<const TreeXmlHashModel*>(index.model());
        QModelIndex parentIndex = index.parent();
        if (parentIndex.data(TreeXmlModel::TagRole) == DBLINKTOFILTERXML::LINKTOFILTER) {
            QModelIndex linkIndex = parentIndex.sibling(
                        parentIndex.row(),
                        hashModel->columnDisplayedAttr(DBLINKTOFILTERXML::LINKTOFILTER,
                                                       DBLINKTOFILTERXML::REFFILTER));
            QModelIndex filterIndex = hashModel->indexHashAttr(DBFILTERXML::FILTER,
                                                               DBFILTERXML::ID,
                                                               linkIndex.data());
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
            QString h = linkIndex.data(Qt::EditRole).toString();
            return classIndex;
        }
    }

    return QModelIndex();
}
