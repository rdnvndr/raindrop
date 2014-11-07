#include "refitemdelegate.h"

#include <QSortFilterProxyModel>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>
#include <treecombobox/treecombobox.h>
#include "dbxmlstruct.h"

RefItemDelegate::RefItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void RefItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor,model,index);
}

QWidget *RefItemDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem
                                       &option, const QModelIndex &index) const
{


    const ModifyProxyModel *modifyModel = dynamic_cast<const ModifyProxyModel *>(index.model());
    const TreeXmlHashModel* hashModel = (modifyModel)?
                dynamic_cast<const TreeXmlHashModel*>(modifyModel->sourceModel())
              : dynamic_cast<const TreeXmlHashModel*>(index.model());

    if (hashModel) {
        QString tag  = index.data(TreeXmlModel::TagRole).toString();
        QString attr = hashModel->displayedAttr(tag, index.column());

        TreeComboBox *comboBoxDestClass = new TreeComboBox(parent);
        QSortFilterProxyModel* classFilterModel = new QSortFilterProxyModel(parent);
        classFilterModel->setFilterKeyColumn(0);
        classFilterModel->setFilterRole(TreeXmlModel::TagRole);
        if (tag == DBLINKTOCLASSXML::LINKTOCLASS) {
            classFilterModel->setFilterRegExp(
                        DBCLASSXML::CLASS + "|" +
                        DBMODELXML::MODEL + "|" +
                        DBCLASSLISTXML::CLASSLIST);
        } else if (tag == DBLINKTOFILTERXML::LINKTOFILTER) {
            classFilterModel->setFilterRegExp(
                        DBFILTERXML::FILTER + "|" +
                        DBCLASSXML::CLASS + "|" +
                        DBMODELXML::MODEL + "|" +
                        DBCLASSLISTXML::CLASSLIST);
        }
        classFilterModel->setSourceModel(
                    const_cast<QAbstractItemModel *>(
                        dynamic_cast<const QAbstractItemModel*>(hashModel)));
        classFilterModel->setDynamicSortFilter(true);
        classFilterModel->sort(0);
        comboBoxDestClass->setModel(classFilterModel);
        comboBoxDestClass->setRootModelIndex(classFilterModel->index(0,0).child(0,0));
        comboBoxDestClass->setIndexColumn(hashModel->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                                      DBATTRXML::ID));
        comboBoxDestClass->setCurrentModelIndex(
                    classFilterModel->index(0,0).child(0,0).child(0,0));
        return comboBoxDestClass;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void RefItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
}
