#include "unitdelegate.h"
#include <treexmlmodel/treexmlhashmodel.h>
#include <metadatamodel/dbxmlstruct.h>
#include <QLineEdit>
#include <QAbstractProxyModel>

using namespace RTPTechGroup::XmlModel;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

UnitDelegate::UnitDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void UnitDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

    QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(editor);
    if (lineEdit) {
        QAbstractProxyModel *proxyModel = dynamic_cast<QAbstractProxyModel*>(model);
        TreeXmlHashModel *hashModel = (proxyModel)?
                    dynamic_cast<TreeXmlHashModel*>(proxyModel->sourceModel())
                  : dynamic_cast<TreeXmlHashModel*>(model);

        if (hashModel) {
            QString tag  = index.data(TreeXmlModel::TagRole).toString();
            QString attr = hashModel->displayedAttr(tag, index.column());

            if (tag == DBUNITXML::UNIT && attr == DBUNITXML::CODE) {
                QString value = lineEdit->text();
                int len = value.length();
                while (len < 4) {
                    value = QString("0") + value;
                    ++len;
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
    Q_UNUSED(option)

    QLineEdit *lineEdit = new QLineEdit(parent);

    const QAbstractProxyModel *proxyModel = dynamic_cast<const QAbstractProxyModel *>(index.model());
    const TreeXmlHashModel *hashModel = (proxyModel)?
                dynamic_cast<const TreeXmlHashModel*>(proxyModel->sourceModel())
              : dynamic_cast<const TreeXmlHashModel*>(index.model());

    if (hashModel) {
        QString tag  = index.data(TreeXmlModel::TagRole).toString();
        QString attr = hashModel->displayedAttr(tag, index.column());
        if (tag == DBUNITXML::UNIT) {
            if (attr == DBUNITXML::CODE)
                lineEdit->setValidator(new QIntValidator(0, 9999, lineEdit));
            else if (attr == DBUNITXML::COEFF || attr == DBUNITXML::DELTA) {
                QRegExpValidator *validator = new QRegExpValidator(lineEdit);
                validator->setRegExp(QRegExp("^[0-9]*[.]?[0-9]*$"));
                lineEdit->setValidator(validator);
            }
        }
    }
    return lineEdit;
}

void UnitDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{ 
    QStyledItemDelegate::setEditorData(editor, index);
}

}}
