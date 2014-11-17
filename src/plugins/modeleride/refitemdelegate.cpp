#include "refitemdelegate.h"

#include <QSortFilterProxyModel>
#include <QLineEdit>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>
#include <treecombobox/treecombobox.h>
#include "dbxmlstruct.h"

RefItemDelegate::RefItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void RefItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    TreeComboBox* comboBoxDestClass = dynamic_cast<TreeComboBox*>(editor);
    if (comboBoxDestClass) {
        ModifyProxyModel* modifyModel = dynamic_cast<ModifyProxyModel*>(model);
        TreeXmlHashModel* hashModel = (modifyModel)?
                    dynamic_cast<TreeXmlHashModel*>(modifyModel->sourceModel())
                  : dynamic_cast<TreeXmlHashModel*>(model);

        if (hashModel) {
            QString valueTag = comboBoxDestClass->currentData(
                        TreeXmlModel::TagRole).toString();
            QString tag  = index.data(TreeXmlModel::TagRole).toString();
            QString attr = hashModel->displayedAttr(tag, index.column());
            QModelIndex i = comboBoxDestClass->currentModelIndex();
            QString data =  i.data(Qt::EditRole).toString();

            if (tag == DBLINKTOFILTERXML::LINKTOFILTER
                    && attr == DBLINKTOFILTERXML::REFFILTER)
            {
                if (valueTag == DBFILTERXML::FILTER)
                    model->setData(index,
                                   data,
                                   Qt::EditRole);
                return;

            }

            if (tag == DBLINKTOCLASSXML::LINKTOCLASS
                    && attr == DBLINKTOCLASSXML::REFCLASS)
            {
                if (valueTag == DBCLASSXML::CLASS)
                    model->setData(index,
                                   data,
                                   Qt::EditRole);
                return;
            }
        }
    }

    QStyledItemDelegate::setModelData(editor,model,index);
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
            if (tag == DBLINKTOFILTERXML::LINKTOFILTER)
                comboBoxDestClass->setIndexColumn(
                            hashModel->columnDisplayedAttr(DBFILTERXML::FILTER,
                                                           DBFILTERXML::ID));
            else
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

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void RefItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
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
            return classIndex;
        }
    }

    return QModelIndex();
}

void RefItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString tag  = index.data(TreeXmlModel::TagRole).toString();
    const ModifyProxyModel *modifyModel = dynamic_cast<const ModifyProxyModel *>(index.model());
    const TreeXmlHashModel* hashModel = (modifyModel)?
                dynamic_cast<const TreeXmlHashModel*>(modifyModel->sourceModel())
              : dynamic_cast<const TreeXmlHashModel*>(index.model());

    if (hashModel) {
        QString attr = hashModel->displayedAttr(tag, index.column());
        if (tag == DBLINKTOFILTERXML::LINKTOFILTER
                && attr == DBLINKTOFILTERXML::REFFILTER)
        {
            QModelIndex filterIndex = hashModel->indexHashAttr(DBFILTERXML::FILTER,
                                                               DBFILTERXML::ID,
                                                               index.data(Qt::EditRole));
            filterIndex = filterIndex.sibling(
                        filterIndex.row(),
                        hashModel->columnDisplayedAttr(DBFILTERXML::FILTER, DBFILTERXML::NAME));
            if (filterIndex.isValid()) {
                QStyledItemDelegate::paint(painter, option, filterIndex);
                return;
            }
        }

        if (tag == DBLINKTOCLASSXML::LINKTOCLASS
                    && attr == DBLINKTOCLASSXML::REFCLASS)
        {
            QModelIndex classIndex = hashModel->indexHashAttr(DBCLASSXML::CLASS,
                                                              DBCLASSXML::ID,
                                                              index.data(Qt::EditRole));
            classIndex = classIndex.sibling(
                        classIndex.row(),
                        hashModel->columnDisplayedAttr(DBCLASSXML::CLASS, DBCLASSXML::NAME));
            if (classIndex.isValid()) {
                QStyledItemDelegate::paint(painter, option, classIndex);
                return;
            }
        }

    }

    QStyledItemDelegate::paint(painter, option, index);
}
