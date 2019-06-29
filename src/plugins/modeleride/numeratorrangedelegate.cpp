#include "numeratorrangedelegate.h"
#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <metadatamodel/dbxmlstruct.h>

#include <QLineEdit>
#include <QComboBox>
#include <QAbstractProxyModel>
#include <QToolTip>

#include "regexpvaluevalidator.h"
#include "treefilterproxymodel.h"
#include "xmldelegate.h"

using namespace RTPTechGroup::XmlModel;
using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

NumeratorRangeDelegate::NumeratorRangeDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

QWidget *NumeratorRangeDelegate::createEditor(QWidget *parent,
                                              const QStyleOptionViewItem &option,
                                              const QModelIndex &index) const
{
    Q_UNUSED(option)

    const QAbstractProxyModel *proxyModel
            = dynamic_cast<const QAbstractProxyModel *>(index.model());
    const TreeXmlHashModel *hashModel = (proxyModel)?
                dynamic_cast<const TreeXmlHashModel*>(proxyModel->sourceModel())
              : dynamic_cast<const TreeXmlHashModel*>(index.model());

    if (hashModel) {
        QString tag  = index.data(TreeXmlModel::TagRole).toString();
        QString attr = hashModel->displayedAttr(tag, index.column());
        if (tag == DBNUMERATORREGEXXML::NUMERATORREGEX
                && attr == DBNUMERATORREGEXXML::REGEX) {
            QLineEdit *lineEdit = new QLineEdit(parent);
            RegExpValueValidator *validator
                    = new RegExpValueValidator(lineEdit);
            lineEdit->setValidator(validator);
            connect(validator, &RegExpValueValidator::stateChanged,
                    this, &NumeratorRangeDelegate::validateRegExp);
            return lineEdit;
        }
        if (tag == DBNUMERATORLOVXML::NUMERATORLOV
                && attr == DBNUMERATORLOVXML::REFLOV) {
            QComboBox *comboBox = new QComboBox(parent);
            comboBox->setItemDelegate(new XmlDelegate(parent));
            TableXMLProxyModel *numeratorFilterModel
                    = new TableXMLProxyModel(parent);
            QStringList tags;
            tags << DBLOVLISTXML::LOVLIST;
            numeratorFilterModel->setAttributeTags(tags);
            numeratorFilterModel->setSourceModel(
                        const_cast<TreeXmlHashModel *>(hashModel));
            numeratorFilterModel->setFilterIndex(hashModel->index(0,0));
            numeratorFilterModel->setFilterRole(Qt::EditRole);
            numeratorFilterModel->setDynamicSortFilter(true);
            numeratorFilterModel->sort(0);

            numeratorFilterModel->setFilterIndex(
                        numeratorFilterModel->mapToSource(
                           childIdx(0,0,numeratorFilterModel->index(0,0)))
                        );
            tags << DBLOVXML::LOV;
            numeratorFilterModel->setAttributeTags(tags);
            comboBox->setModel(numeratorFilterModel);
            comboBox->setRootModelIndex(
                        childIdx(0,0,numeratorFilterModel->index(0,0)));
            comboBox->setCurrentIndex(-1);
            return comboBox;
        }
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

QModelIndex NumeratorRangeDelegate::childIdx(int arow, int acolumn, const QModelIndex &parent) const
{
    return parent.model() ? parent.model()->index(arow, acolumn, parent)
                          : QModelIndex();
}

void NumeratorRangeDelegate::validateRegExp(QValidator::State state) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(QObject::sender()->parent());
    if (lineEdit != nullptr) {
        if(state != QValidator::Acceptable)
            QToolTip::showText(lineEdit->mapToGlobal(QPoint(0,5)),
                               tr("Введено неверное\n"
                                  "регулярное выражение."));
        else
            QToolTip::hideText();
    }
}

}}
