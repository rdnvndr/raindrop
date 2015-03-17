#include "proprole.h"

#include <QMdiSubWindow>

#include <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropRole::PropRole(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);
}

PropRole::~PropRole()
{

}

void PropRole::setModel(TreeXmlHashModel *model)
{

    AbstractPropEditor::setModel(model);
}

void PropRole::setCurrent(const QModelIndex &index)
{

}

void PropRole::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString roleName = modelData(DBROLEXML::ROLE, DBROLEXML::NAME,index).toString();
    QString id = modelData(DBROLEXML::ROLE, DBROLEXML::ID,index).toString();

    this->setObjectName("PropRole::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(roleName);
}

void PropRole::edit(bool flag)
{

}

}}
