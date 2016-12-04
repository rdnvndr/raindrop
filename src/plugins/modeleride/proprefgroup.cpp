#include "proprefgroup.h"

#include <QMdiSubWindow>
#include <QMessageBox>
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropRefGroup::PropRefGroup(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);

    connect(refGroupWidget, &AbstractEditorWidget::currentIndexChanged,
            this, &PropRefGroup::setTabName);
    connect(refGroupWidget, &AbstractEditorWidget::dataChanged,
            this, &PropRefGroup::setTabName);
    connect(refGroupWidget, &AbstractEditorWidget::dataAboutToBeRemoved,
            this, &AbstractPropEditor::closeTab);
}

PropRefGroup::~PropRefGroup()
{

}

void PropRefGroup::setModel(TreeXmlHashModel *model)
{
    refGroupWidget->setModel(model);

    AbstractPropEditor::setModel(model);
}

QString PropRefGroup::dataId(const QModelIndex &index)
{
    return modelData(DBREFGROUPXML::REFGROUP,
                     DBREFGROUPXML::ID,
                     index).toString();
}

void PropRefGroup::setCurrent(const QModelIndex &index)
{
    refGroupWidget->setCurrent(index);
}

void PropRefGroup::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString refGroupName = modelData(
                DBREFGROUPXML::REFGROUP,
                DBREFGROUPXML::NAME,index).toString();
    QString id = this->dataId(index);

    this->setObjectName("PropRefGroup::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(refGroupName);
}

void PropRefGroup::edit(bool flag)
{
    refGroupWidget->edit(flag);
}

}}
