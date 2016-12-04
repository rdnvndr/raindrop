#include "propref.h"
#include <QMdiSubWindow>
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropRef::PropRef(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);

    connect(refWidget, &AbstractEditorWidget::currentIndexChanged,
            refItemWidget, &AbstractModifyWidget::setRootIndex);
    connect(refWidget, &AbstractEditorWidget::currentIndexChanged,
            this, &PropRef::setTabName);
    connect(refWidget, &AbstractEditorWidget::dataChanged,
            this, &PropRef::setTabName);
    connect(refWidget, &AbstractEditorWidget::dataAboutToBeRemoved,
            this, &AbstractPropEditor::closeTab);

    connect(refWidget, &RefWidget::edited, this, &PropRef::edit);
    connect(refWidget, &RefWidget::edited, refItemWidget, &RefItemWidget::edit);
    connect(toolButtonAdd, &QToolButton::clicked,
            refWidget, &RefWidget::add);
    connect(toolButtonDel, &QToolButton::clicked,
            refItemWidget, &RefItemWidget::revert);
    connect(toolButtonDel, &QToolButton::clicked,
            refWidget, &RefWidget::remove);
    connect(toolButtonEdit,&QToolButton::clicked,
            refWidget, &RefWidget::edit);

    connect(pushButtonPropCancel, &QPushButton::clicked,
            refItemWidget, &RefItemWidget::revert);
    connect(pushButtonPropCancel, &QPushButton::clicked,
            refWidget, &RefWidget::revert);
    connect(pushButtonPropSave, &QPushButton::clicked,
            refItemWidget, &RefItemWidget::submit);
    connect(pushButtonPropSave, &QPushButton::clicked,
            refWidget, &RefWidget::submit);
}

PropRef::~PropRef()
{

}

void PropRef::setModel(TreeXmlHashModel *model)
{
    refWidget->setModel(model);
    refItemWidget->setModel(model);

    AbstractPropEditor::setModel(model);
}

QString PropRef::dataId(const QModelIndex &index)
{
    return modelData(DBREFXML::REF, DBREFXML::ID,index).toString();
}

void PropRef::setCurrent(const QModelIndex &index)
{
    refWidget->setCurrent(index);
}

void PropRef::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString refItemName = modelData(DBREFXML::REF, DBREFXML::NAME,index).toString();
    QString id = this->dataId(index);

    this->setObjectName("PropRef::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(refItemName);
}

void PropRef::edit(bool flag)
{
    if (refWidget->isEmpty()){
        toolButtonAdd->setDisabled(true);
    } else
        toolButtonAdd->setEnabled(true);

    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEdit->setDisabled(flag);
}

}}
