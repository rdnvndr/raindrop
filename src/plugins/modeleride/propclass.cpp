#include <QMdiSubWindow>
#include <metadatamodel/dbxmlstruct.h>
#include "propclass.h"
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropClass::PropClass(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);

    connect(classWidget, &AbstractEditorWidget::currentIndexChanged,
            attrWidget, &AttrWidget::setRootIndex);
    connect(classWidget, &AbstractEditorWidget::currentIndexChanged,
            compWidget, &AbstractItemWidget::setRootIndex);
    connect(classWidget, &AbstractEditorWidget::currentIndexChanged,
            filterWidget, &AbstractItemWidget::setRootIndex);
    connect(classWidget, &AbstractEditorWidget::currentIndexChanged,
            this, &PropClass::setTabName);
    connect(classWidget, &AbstractEditorWidget::currentIndexChanged,
            permWidget, &PermissionWidget::setRootIndex);

    connect(classWidget, &AbstractEditorWidget::dataChanged,
            this, &PropClass::setTabName);
    connect(classWidget, &AbstractEditorWidget::dataAboutToBeRemoved,
            this, &AbstractPropEditor::closeTab);
    connect(filterWidget, &FilterWidget::dataEdited,
            this, &PropClass::onEditFilter);
}

PropClass::~PropClass()
{
    if (classWidget->isEdit()) classWidget->revert();
    if (attrWidget->isEdit())  attrWidget->revert();
}

void PropClass::setModel(TreeXmlHashModel *model)
{
    classWidget->setModel(model);
    attrWidget->setModel(model);
    compWidget->setModel(model);
    filterWidget->setModel(model);
    permWidget->setModel(model);
    AbstractPropEditor::setModel(model);
}

QString PropClass::dataId(const QModelIndex &index)
{
    return modelData(DBCLASSXML::CLASS, DBCLASSXML::ID,index).toString();
}

void PropClass::setCurrent(const QModelIndex &index)
{
    classWidget->setCurrent(index);
}

void PropClass::setTabName(const QModelIndex &index){
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    if (!subWindow) return;

    QString className = modelData(DBCLASSXML::CLASS, DBCLASSXML::NAME,index).toString();
    QString id = this->dataId(index);

    this->setObjectName("PropClass::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(className);
}

void PropClass::edit(bool flag)
{
    classWidget->edit(flag);
}

void PropClass::onEditComposition(const QModelIndex &index)
{
    emit editComposition(index);
}


void PropClass::onEditFilter(const QModelIndex &index)
{
    emit editFilter(index);
}

}}
