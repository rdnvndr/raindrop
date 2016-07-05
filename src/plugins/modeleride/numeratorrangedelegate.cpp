#include "numeratorrangedelegate.h"
#include <treexmlmodel/treexmlhashmodel.h>
#include <metadatamodel/dbxmlstruct.h>

#include <QLineEdit>
#include <QAbstractProxyModel>
#include <QToolTip>

#include "regexpvaluevalidator.h"

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

    QLineEdit *lineEdit = new QLineEdit(parent);

    const QAbstractProxyModel *proxyModel = dynamic_cast<const QAbstractProxyModel *>(index.model());
    const TreeXmlHashModel *hashModel = (proxyModel)?
                dynamic_cast<const TreeXmlHashModel*>(proxyModel->sourceModel())
              : dynamic_cast<const TreeXmlHashModel*>(index.model());

    if (hashModel) {
        QString tag  = index.data(TreeXmlModel::TagRole).toString();
        QString attr = hashModel->displayedAttr(tag, index.column());
        if (tag == DBNUMERATORRANGEXML::NUMERATORRANGE) {
            if (attr == DBNUMERATORRANGEXML::REGEX)
            {
                RegExpValueValidator *validator = new RegExpValueValidator(lineEdit);
                lineEdit->setValidator(validator);
                connect(validator,SIGNAL(stateChanged(QValidator::State)),
                        this,SLOT(validateRegExp(QValidator::State)));
            }
        }
    }
    return lineEdit;
}

void NumeratorRangeDelegate::validateRegExp(QValidator::State state) const
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(QObject::sender()->parent());
    if (lineEdit != NULL) {
        if(state != QValidator::Acceptable)
            QToolTip::showText(lineEdit->mapToGlobal(QPoint(0,5)),
                               tr("Введено неверное\n"
                                  "регулярное выражение."));
        else
            QToolTip::hideText();
    }
}

}}
