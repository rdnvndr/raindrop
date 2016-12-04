#include "proplov.h"
#include <QMdiSubWindow>
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropLov::PropLov(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);

    connect(lovWidget, &AbstractEditorWidget::currentIndexChanged,
            lovValueWidget,&AbstractModifyWidget::setRootIndex);
    connect(lovWidget, &AbstractEditorWidget::currentIndexChanged,
            this, &PropLov::setTabName);
    connect(lovWidget, &AbstractEditorWidget::dataChanged,
            this, &PropLov::setTabName);
    connect(lovWidget, &AbstractEditorWidget::dataAboutToBeRemoved,
            this, &AbstractPropEditor::closeTab);

    connect(lovWidget, &LovWidget::edited, this, &PropLov::edit);
    connect(lovWidget, &LovWidget::edited, lovValueWidget, &LovValueWidget::edit);

    connect(toolButtonAddLov, &QToolButton::clicked,
            lovWidget, &LovWidget::add);
    connect(toolButtonDelLov, &QToolButton::clicked,
            lovValueWidget, &LovValueWidget::revert);
    connect(toolButtonDelLov, &QToolButton::clicked,
            lovWidget, &LovWidget::remove);
    connect(toolButtonEditLov, &QToolButton::clicked,
            lovWidget, &LovWidget::edit);

    connect(pushButtonPropCancel, &QPushButton::clicked,
            lovValueWidget, &LovValueWidget::revert);
    connect(pushButtonPropCancel, &QPushButton::clicked,
            lovWidget, &LovWidget::revert);
    connect(pushButtonPropSave, &QPushButton::clicked,
            lovValueWidget, &LovValueWidget::submit);
    connect(pushButtonPropSave, &QPushButton::clicked,
            lovWidget, &LovWidget::submit);
}

PropLov::~PropLov()
{

}

void PropLov::setModel(TreeXmlHashModel *model)
{
    lovWidget->setModel(model);
    lovValueWidget->setModel(model);

    AbstractPropEditor::setModel(model);
}

QString PropLov::dataId(const QModelIndex &index)
{
    return modelData(DBLOVXML::LOV, DBLOVXML::ID,index).toString();
}

void PropLov::setCurrent(const QModelIndex &index)
{
    lovWidget->setCurrent(index);
}

void PropLov::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString lovName = modelData(DBLOVXML::LOV, DBLOVXML::NAME,index).toString();
    QString id = this->dataId(index);

    this->setObjectName("PropLov::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(lovName);
}

void PropLov::edit(bool flag)
{
    if (lovWidget->isEmpty()){
        toolButtonAddLov->setDisabled(true);
    } else
        toolButtonAddLov->setEnabled(true);

    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEditLov->setDisabled(flag);
}

}}
