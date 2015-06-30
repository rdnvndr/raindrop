#include "propentitygroup.h"

#include <QMdiSubWindow>
#include <QMessageBox>
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropEntityGroup::PropEntityGroup(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);

    connect(entityGroupWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(entityGroupWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(entityGroupWidget,SIGNAL(dataAboutToBeRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));
}

PropEntityGroup::~PropEntityGroup()
{

}

void PropEntityGroup::setModel(TreeXmlHashModel *model)
{
    entityGroupWidget->setModel(model);

    AbstractPropEditor::setModel(model);
}

QString PropEntityGroup::dataId(const QModelIndex &index)
{
    return modelData(DBQUANTITYGROUPXML::QUANTITYGROUP,
                               DBQUANTITYGROUPXML::ID,
                               index).toString();
}

void PropEntityGroup::setCurrent(const QModelIndex &index)
{
    entityGroupWidget->setCurrent(index);
}

void PropEntityGroup::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString entityGroupName = modelData(
                DBQUANTITYGROUPXML::QUANTITYGROUP,
                DBQUANTITYGROUPXML::NAME,index).toString();
    QString id = this->dataId(index);

    this->setObjectName("PropEntityGroup::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(entityGroupName);
}

void PropEntityGroup::edit(bool flag)
{
    entityGroupWidget->edit(flag);
}

}}
