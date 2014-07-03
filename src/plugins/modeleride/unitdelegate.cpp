#include "unitdelegate.h"
#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>
#include "dbxmlstruct.h"
#include <QLineEdit>


UnitDelegate::UnitDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void UnitDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(editor);
    if (lineEdit) {
        ModifyProxyModel* modifyModel = dynamic_cast<ModifyProxyModel*>(model);
        TreeXmlHashModel* hashModel = (modifyModel)?
                    dynamic_cast<TreeXmlHashModel*>(modifyModel->sourceModel())
                  : dynamic_cast<TreeXmlHashModel*>(model);

        if (hashModel) {
            QString tag  = index.data(Qt::UserRole).toString();
            QString attr = hashModel->displayedAttr(tag, index.column());

            if (tag == DBUNITXML::UNIT && attr == DBUNITXML::CODE) {
                QString value = lineEdit->text();
                int len = value.length();
                while (len < 4) {
                    value = QString("0") + value;
                    len++;
                }
                model->setData(index, value, Qt::EditRole);
                return;
            }
        }
    }

    QStyledItemDelegate::setModelData(editor,model,index);
}

QWidget *UnitDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QWidget *widget = QStyledItemDelegate::createEditor(parent,option,index);

    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(widget);
    if (lineEdit) {
        const ModifyProxyModel *modifyModel = dynamic_cast<const ModifyProxyModel *>(index.model());
        const TreeXmlHashModel* hashModel = (modifyModel)?
                    dynamic_cast<const TreeXmlHashModel*>(modifyModel->sourceModel())
                  : dynamic_cast<const TreeXmlHashModel*>(index.model());

        if (hashModel) {
            QString tag  = index.data(Qt::UserRole).toString();
            QString attr = hashModel->displayedAttr(tag, index.column());
            if (tag == DBUNITXML::UNIT && attr == DBUNITXML::CODE) {
                lineEdit->setValidator(new QIntValidator(0, 9999, lineEdit));
            }
        }
    }

    return widget;
}

void UnitDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{ 
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(editor);
    if (lineEdit) {
        lineEdit->setMaxLength(16);
    }
    QStyledItemDelegate::setEditorData(editor, index);
}
