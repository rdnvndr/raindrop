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

    connect(numeratorWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            numeratorRangeWidget,SLOT(setRootIndex(QModelIndex)));
    connect(numeratorWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(numeratorWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(numeratorWidget,SIGNAL(dataAboutToBeRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));

    connect(numeratorWidget, SIGNAL(edited(bool)), this, SLOT(edit(bool)));
    connect(numeratorWidget, SIGNAL(edited(bool)), numeratorRangeWidget, SLOT(edit(bool)));

    connect(toolButtonAddNumerator,  SIGNAL(clicked()), numeratorWidget, SLOT(add()));
    connect(toolButtonDelNumerator,  SIGNAL(clicked()), numeratorRangeWidget, SLOT(revert()));
    connect(toolButtonDelNumerator,  SIGNAL(clicked()), numeratorWidget, SLOT(remove()));
    connect(toolButtonEditNumerator, SIGNAL(clicked()), numeratorWidget, SLOT(edit()));

    connect(pushButtonPropCancel, SIGNAL(clicked()), numeratorRangeWidget, SLOT(revert()));
    connect(pushButtonPropCancel, SIGNAL(clicked()), numeratorWidget, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), numeratorRangeWidget, SLOT(submit()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), numeratorWidget, SLOT(submit()));
}

PropNumerator::~PropNumerator()
{

}

void PropNumerator::setModel(TreeXmlHashModel *model)
{
    numeratorWidget->setModel(model);
    numeratorRangeWidget->setModel(model);

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
    numeratorWidget->setCurrent(index);
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
    if (numeratorWidget->isEmpty()){
        toolButtonAddNumerator->setDisabled(true);
    } else
        toolButtonAddNumerator->setEnabled(true);

    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEditNumerator->setDisabled(flag);
}

}}
