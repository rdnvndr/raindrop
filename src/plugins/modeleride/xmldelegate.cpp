#include "xmldelegate.h"
#include "pushbuttonimage.h"
#include <QComboBox>
#include <QCheckBox>
#include <QAbstractItemView>
#include <QLineEdit>
#include <QApplication>
#include <QSize>
#include <QPainter>
#include <treecombobox/treecombobox.h>
#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>

XmlDelegate::XmlDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget*  XmlDelegate::createEditor (QWidget * parent, const QStyleOptionViewItem & option,
        const QModelIndex & index ) const
{
    // Для логического типа создаем QCheckbox
    if (index.data(Qt::EditRole).type() == QVariant::Bool) {
        QCheckBox* pRes = new  QCheckBox(parent);
        return pRes;
    }

    return QStyledItemDelegate::createEditor(parent,option,index);
}

void XmlDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    if (index.isValid()) {
        // Логическое
        if (index.data(Qt::EditRole).type() == QVariant::Bool) {
            QCheckBox *pRes = static_cast<QCheckBox*>(editor);
            pRes->setChecked(index.model()->data(index, Qt::EditRole).toBool());
            return;
        }

        TreeComboBox* treeComboBox = dynamic_cast<TreeComboBox*>(editor);
        if (treeComboBox) {
            treeComboBox->setDisplayText(
                        index.model()->data(index,Qt::DisplayRole).toString());

            treeComboBox->repaint();
            return;
        }

        QComboBox* comboBox = dynamic_cast<QComboBox*>(editor);
        if (comboBox) {
            TreeXmlHashModel* hashModel = this->sourceModel(comboBox->model());
            TreeXmlHashModel* indexModel = this->sourceModel(
                        const_cast<QAbstractItemModel*>(index.model()));
            if (hashModel && hashModel == indexModel) {
                QModelIndex curIndex = mapFromSource(comboBox->model(),
                                                     hashModel->indexLink(
                                                         mapToSource(index)
                                                         ));
                comboBox->setCurrentIndex(curIndex.row());
                return;
            }

            comboBox->setEditText(index.model()->data(index,Qt::EditRole).toString());
            return;
        }

        QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(editor);
        if (lineEdit) {
            if (index.isValid())
                lineEdit->setText(
                            index.model()->data(index,Qt::DisplayRole).toString());
            return;
        }

        PushButtonImage *pushButton = dynamic_cast<PushButtonImage*>(editor);
        if (pushButton) {
            QByteArray bytes = QByteArray::fromBase64(index.data().toByteArray());
            pushButton->setData(bytes);
            return;
        }
    }

    QStyledItemDelegate::setEditorData(editor, index);
    return;

}

void XmlDelegate::setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    // Логическое
    if (index.data(Qt::EditRole).type() == QVariant::Bool) {
        QCheckBox *pRes = static_cast<QCheckBox*>(editor);
        model->setData(index,pRes->isChecked(),Qt::EditRole);
        return;
    }

    TreeComboBox* treeComboBox = dynamic_cast<TreeComboBox*>(editor);
    if (treeComboBox) {
        if (treeComboBox->currentModelIndex().isValid())
            model->setData(index,treeComboBox->currentModelIndex().data(Qt::EditRole),Qt::EditRole);
        return;
    }
    // Если QComboBox присваиваем текущий текст, а не индекс
    QComboBox* comboBox = dynamic_cast<QComboBox*>(editor);
    if (comboBox) {

        TreeXmlHashModel* hashModel =  this->sourceModel(comboBox->model());
        TreeXmlHashModel* indexModel = this->sourceModel(
                    const_cast<QAbstractItemModel*>(model));

        if (hashModel && hashModel == indexModel) {
            QString tag  = index.data(TreeXmlModel::TagRole).toString();
            QString attr = hashModel->displayedAttr(tag, mapToSource(index).column());
            int column = hashModel->columnDisplayedAttr(hashModel->toRelation(tag,attr).tag,
                                                        hashModel->uuidAttr(tag));

            QModelIndex curIndex = comboBox->view()->currentIndex();
            curIndex = mapToSource(curIndex);
            curIndex = curIndex.sibling(curIndex.row(), column);
            if (curIndex.isValid())
                model->setData(index, curIndex.data(Qt::EditRole), Qt::EditRole);

            return;
        }
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

//void XmlDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
//{
//    editor->setGeometry(option.rect);
//}

QRect  XmlDelegate::getCenteredComboBoxRect(const QStyleOptionViewItem &option) const
{
    QSize size = QApplication::style()->sizeFromContents(QStyle::CT_CheckBox, &option, QSize());
#ifdef Q_OS_WIN32
    size.setWidth(size.width() + QApplication::style()->pixelMetric(QStyle::PM_CheckBoxLabelSpacing, &option));
#endif

    QPoint center = option.rect.center();
    QRect result;
    result.setTop(center.y() - size.height() / 2);
    result.setLeft(center.x() - size.width() / 2);
    result.setSize(size);

    return result;
}

void XmlDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Логический
    if (index.model()->data(index, Qt::EditRole).type() == QVariant::Bool){
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);
        opt.text.clear();

        QStyleOptionButton checkBoxOption;

        checkBoxOption.rect = getCenteredComboBoxRect(option);
        checkBoxOption.state = opt.state | (index.data().toBool() ? QStyle::State_On : QStyle::State_Off);

        if (!index.flags().testFlag(Qt::ItemIsEditable))
            checkBoxOption.state = checkBoxOption.state & ~QStyle::State_Enabled;
        // Рисуем фон
        QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxOption, painter);
        return;
    }

    // Вывод ссылок
    const ModifyProxyModel *modifyModel = dynamic_cast<const ModifyProxyModel *>(index.model());
    const TreeXmlHashModel* hashModel = (modifyModel)?
                dynamic_cast<const TreeXmlHashModel*>(modifyModel->sourceModel())
              : dynamic_cast<const TreeXmlHashModel*>(index.model());
    if (hashModel) {
        QString tag  = index.data(TreeXmlModel::TagRole).toString();
        QString attr = hashModel->displayedAttr(tag, index.column());
        QModelIndex linkIndex  = hashModel->indexLink(tag, attr, index.data());
        if (linkIndex.isValid()) {
            QStyledItemDelegate::paint(painter, option, linkIndex);
            return;
        }
    }

    QStyledItemDelegate::paint(painter, option, index);
}

TreeXmlHashModel *XmlDelegate::sourceModel(QAbstractItemModel *model) const
{
    ModifyProxyModel* modifyModel = dynamic_cast<ModifyProxyModel*>(model);

    if (modifyModel)
        return dynamic_cast<TreeXmlHashModel*>(modifyModel->sourceModel());

    QAbstractProxyModel *proxyModel = dynamic_cast<QAbstractProxyModel*>(model);
    if (proxyModel)
        return dynamic_cast<TreeXmlHashModel*>(proxyModel->sourceModel());

    return dynamic_cast<TreeXmlHashModel*>(model);
}

QModelIndex XmlDelegate::mapToSource(QModelIndex index) const
{
    QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());

    ModifyProxyModel* modifyModel = dynamic_cast<ModifyProxyModel*>(model);
    if (modifyModel)
        return modifyModel->mapToSource(index);

    QAbstractProxyModel *proxyModel = dynamic_cast<QAbstractProxyModel*>(model);
    if (proxyModel)
        return proxyModel->mapToSource(index);

    return index;
}

QModelIndex XmlDelegate::mapFromSource(QAbstractItemModel *toModel, QModelIndex index) const
{
    ModifyProxyModel    *modifyModel = dynamic_cast<ModifyProxyModel*>(toModel);
    if (modifyModel)
        return modifyModel->mapFromSource(index);

    QAbstractProxyModel *proxyModel  = dynamic_cast<QAbstractProxyModel*>(toModel);
    if (proxyModel)
        return proxyModel->mapFromSource(index);

    return index;
}
