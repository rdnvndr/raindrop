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

    connect(lovWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            lovValueWidget,SLOT(setRootIndex(QModelIndex)));
    connect(lovWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(lovWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(lovWidget,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));

    connect(lovWidget, SIGNAL(edited(bool)), this, SLOT(edit(bool)));
    connect(lovWidget, SIGNAL(edited(bool)), lovValueWidget, SLOT(edit(bool)));

    connect(toolButtonAddLov,  SIGNAL(clicked()), lovWidget, SLOT(add()));
    connect(toolButtonDelLov,  SIGNAL(clicked()), lovValueWidget, SLOT(revert()));
    connect(toolButtonDelLov,  SIGNAL(clicked()), lovWidget, SLOT(remove()));
    connect(toolButtonEditLov, SIGNAL(clicked()), lovWidget, SLOT(edit()));

    connect(pushButtonPropCancel, SIGNAL(clicked()), lovValueWidget, SLOT(revert()));
    connect(pushButtonPropCancel, SIGNAL(clicked()), lovWidget, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), lovValueWidget, SLOT(submit()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), lovWidget, SLOT(submit()));
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
