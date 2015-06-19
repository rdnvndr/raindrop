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

    connect(refWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            refItemWidget,SLOT(setRootIndex(QModelIndex)));
    connect(refWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(refWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(refWidget,SIGNAL(dataAboutToBeRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));

    connect(refWidget, SIGNAL(edited(bool)), this, SLOT(edit(bool)));
    connect(refWidget, SIGNAL(edited(bool)), refItemWidget, SLOT(edit(bool)));
    connect(toolButtonAdd,  SIGNAL(clicked()), refWidget, SLOT(add()));
    connect(toolButtonDel,  SIGNAL(clicked()), refItemWidget, SLOT(revert()));
    connect(toolButtonDel,  SIGNAL(clicked()), refWidget, SLOT(remove()));
    connect(toolButtonEdit, SIGNAL(clicked()), refWidget, SLOT(edit()));

    connect(pushButtonPropCancel, SIGNAL(clicked()), refItemWidget, SLOT(revert()));
    connect(pushButtonPropCancel, SIGNAL(clicked()), refWidget, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), refItemWidget, SLOT(submit()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), refWidget, SLOT(submit()));
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
