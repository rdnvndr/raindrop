#include "xmldelegate.h"
#include "pushbuttonimage.h"
#include <QComboBox>
#include <QDebug>
#include <QAbstractItemView>
#include <QLineEdit>
#include <treecombobox/treecombobox.h>
#include <treexmlmodel/treexmlhashmodel.h>

XmlDelegate::XmlDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget*  XmlDelegate::createEditor (QWidget * parent, const QStyleOptionViewItem & option,
        const QModelIndex & index ) const
{
    return QStyledItemDelegate::createEditor(parent,option,index);
}

void XmlDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{

    TreeComboBox* treeComboBox = dynamic_cast<TreeComboBox*>(editor);
    if (treeComboBox) {
        treeComboBox->setDisplayText(
                    index.model()->data(index,Qt::DisplayRole).toString());

        treeComboBox->repaint();
        return;
    }

    QComboBox* comboBox = dynamic_cast<QComboBox*>(editor);
    if (comboBox) {
        TreeXmlHashModel* hashModel = dynamic_cast<TreeXmlHashModel*>(comboBox->model());
        if (hashModel && hashModel == index.model()) {
            QModelIndex curIndex = hashModel->indexLink(index);
            comboBox->setCurrentIndex(curIndex.row());
        } else
            comboBox->setEditText(index.model()->data(index,Qt::EditRole).toString());

        return;
    }

    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(editor);
    if (lineEdit) {
        lineEdit->setText(index.model()->data(index,Qt::DisplayRole).toString());
        return;
    }

    PushButtonImage *pushButton = dynamic_cast<PushButtonImage*>(editor);
    if (pushButton) {
        QByteArray bytes = QByteArray::fromBase64(index.data().toByteArray());
        pushButton->setData(bytes);
        return;
    }

    QStyledItemDelegate::setEditorData(editor, index);
    return;

}

void XmlDelegate::setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index )const
{
    TreeComboBox* treeComboBox = dynamic_cast<TreeComboBox*>(editor);
    if (treeComboBox) {
        if (treeComboBox->currentModelIndex().isValid())
            model->setData(index,treeComboBox->currentModelIndex().data(),Qt::EditRole);
        return;
    }
    // Если QComboBox присваиваем текущий текст, а не индекс
    QComboBox* comboBox = dynamic_cast<QComboBox*>(editor);
    if (comboBox) {
        TreeXmlHashModel* hashModel = dynamic_cast<TreeXmlHashModel*>(comboBox->model());
        if (hashModel && hashModel == model) {
            QString tag  = index.data(Qt::UserRole).toString();
            QString attr = hashModel->displayedAttr(tag, index.column());
            int column = hashModel->columnDisplayedAttr(hashModel->toRelation(tag,attr).tag,
                                                        hashModel->uuidAttr(tag));
            QModelIndex curIndex = comboBox->view()->currentIndex();
            model->setData(index,
                           curIndex.sibling(curIndex.row(),
                           column).data(Qt::EditRole),
                           Qt::EditRole);
        } else
            model->setData(index,comboBox->currentText(),Qt::EditRole);
        return;
    }

    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(editor);
    if (lineEdit)
        if (lineEdit->isReadOnly()) {
            model->setData(index,index.data(Qt::EditRole),Qt::EditRole);
            return;
        }

    PushButtonImage *pushButton = dynamic_cast<PushButtonImage*>(editor);
    if (pushButton) {
        model->setData(index,pushButton->data().toBase64(),Qt::EditRole);
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
