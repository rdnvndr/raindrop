#include "xmldelegate.h"
#include <QComboBox>
#include <QDebug>
#include <QAbstractItemView>
#include <treecombobox/treecombobox.h>

XmlDelegate::XmlDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget*  XmlDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option,
        const QModelIndex & index ) const
{
    return QStyledItemDelegate::createEditor(parent,option,index);
}

void XmlDelegate::setEditorData( QWidget * editor, const QModelIndex & index )const
{

    TreeComboBox* treeComboBox = dynamic_cast<TreeComboBox*>(editor);
    if (treeComboBox) {
        treeComboBox->setDisplayText(index.model()->data(index,Qt::EditRole).toString());
        if (!treeComboBox->isEnabled())
            treeComboBox->repaint();
        return;
    }

    QComboBox* comboBox = dynamic_cast<QComboBox*>(editor);
    if (comboBox) {
        comboBox->setEditText(index.model()->data(index,Qt::EditRole).toString());
        return;
    }

    QStyledItemDelegate::setEditorData(editor, index);
    return;

}

void XmlDelegate::setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index )const
{
    // Если QComboBox присваиваем текущий текст, а не индекс
    QComboBox* comboBox = dynamic_cast<QComboBox*>(editor);
    if (comboBox) {
        model->setData(index,comboBox->currentText(),Qt::EditRole);
        return;
    }

    QStyledItemDelegate::setModelData(editor,model,index);
    return;
}

void XmlDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

void XmlDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}
