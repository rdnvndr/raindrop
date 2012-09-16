#include "xmldelegate.h"
#include <QComboBox>
#include <QDebug>
#include <QAbstractItemView>

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
    QComboBox* pRes = dynamic_cast<QComboBox*>(editor);
    if (pRes) {
        pRes->setEditText(index.model()->data(index,Qt::EditRole).toString());
        return;
    }
    QStyledItemDelegate::setEditorData(editor, index);
    return;

}

void XmlDelegate::setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index )const
{
    // Если QComboBox присваиваем текущий текст, а не индекс
    QComboBox* pRes = dynamic_cast<QComboBox*>(editor);
    if (pRes) {
        model->setData(index,pRes->currentText(),Qt::EditRole);
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
