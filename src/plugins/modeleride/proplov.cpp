#include "proplov.h"
#include <QMdiSubWindow>
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropLov::PropLov(QWidget *parent) :
    QWidget(parent)
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

    m_model = model;
}

TreeXmlHashModel *PropLov::model()
{
    return m_model;
}

void PropLov::setCurrent(const QModelIndex &index)
{
    lovWidget->setCurrent(index);
}

void PropLov::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString lovName = modelData(DBLOVXML::LOV, DBLOVXML::NAME,index).toString();
    QString id = modelData(DBLOVXML::LOV, DBLOVXML::ID,index).toString();

    this->setObjectName("PropLov::" + id);
    subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
    subWindow->setWindowTitle(lovName);
}

void PropLov::closeTab(const QModelIndex &index)
{
    Q_UNUSED(index);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    subWindow->close();
}

QVariant PropLov::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                      tag,attr)).data();
}

void PropLov::edit(bool flag)
{
    if (lovWidget->isEmpty()){
        toolButtonAddLov->setDisabled(true);
        flag = true;
    } else
        toolButtonAddLov->setEnabled(true);

    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEditLov->setDisabled(flag);
}

}}
