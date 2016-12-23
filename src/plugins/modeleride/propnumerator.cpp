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

    connect(numeratorWidget, &AbstractEditorWidget::currentIndexChanged,
            numeratorRangeWidget, &AbstractModifyWidget::setRootIndex);
    connect(numeratorWidget, &AbstractEditorWidget::currentIndexChanged,
            this, &PropNumerator::setTabName);
    connect(numeratorWidget, &AbstractEditorWidget::dataChanged,
            this, &PropNumerator::setTabName);
    connect(numeratorWidget, &AbstractEditorWidget::dataAboutToBeRemoved,
            this, &AbstractPropEditor::closeTab);

    connect(numeratorWidget, &NumeratorWidget::edited, this,
            static_cast<void (AbstractPropEditor::*)(bool flag)>(&AbstractPropEditor::edit));
    connect(numeratorWidget, &NumeratorWidget::edited, numeratorRangeWidget,
            static_cast<void (AbstractModifyWidget::*)(bool flag)>(&AbstractModifyWidget::edit));

    connect(toolButtonAddNumerator, &QToolButton::clicked,
            numeratorWidget, &NumeratorWidget::add);
    connect(toolButtonDelNumerator, &QToolButton::clicked,
            numeratorRangeWidget, &NumeratorRangeWidget::revert);
    connect(toolButtonDelNumerator, &QToolButton::clicked,
            numeratorWidget, &NumeratorWidget::remove);
    connect(toolButtonEditNumerator, &QToolButton::clicked, numeratorWidget,
            static_cast<void (AbstractEditorWidget::*)()>(&AbstractEditorWidget::edit));

    connect(pushButtonPropCancel, &QPushButton::clicked,
            numeratorRangeWidget, &NumeratorRangeWidget::revert);
    connect(pushButtonPropCancel, &QPushButton::clicked,
            numeratorWidget, &NumeratorWidget::revert);
    connect(pushButtonPropSave, &QPushButton::clicked,
            numeratorRangeWidget, &NumeratorRangeWidget::submit);
    connect(pushButtonPropSave, &QPushButton::clicked,
            numeratorWidget, &NumeratorWidget::submit);
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
