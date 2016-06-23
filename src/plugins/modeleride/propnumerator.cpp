#include "propnumerator.h"

#include <QMdiSubWindow>
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropNumerator::PropNumerator(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);

}

PropNumerator::~PropNumerator()
{

}

void PropNumerator::setModel(TreeXmlHashModel *model)
{
    AbstractPropEditor::setModel(model);
}

QString PropNumerator::dataId(const QModelIndex &index)
{
    return modelData(DBNUMERATORXML::NUMERATOR,
                     DBNUMERATORXML::ID,
                     index).toString();
}

void PropNumerator::setCurrent(const QModelIndex &index)
{

}

void PropNumerator::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString numeratorName = modelData(DBNUMERATORXML::NUMERATOR,
                                      DBNUMERATORXML::NAME,
                                      index).toString();
    QString id = this->dataId(index);

    this->setObjectName("PropNumerator::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(numeratorName);
}

void PropNumerator::edit(bool flag)
{

}

}}
