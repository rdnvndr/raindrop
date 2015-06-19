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

    connect(classWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            attrWidget,SLOT(setRootIndex(QModelIndex)));
    connect(classWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            compWidget,SLOT(setRootIndex(QModelIndex)));
    connect(classWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            filterWidget,SLOT(setRootIndex(QModelIndex)));
    connect(classWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(classWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            permWidget,SLOT(setRootIndex(QModelIndex)));

    connect(classWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(classWidget,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));
    connect(filterWidget,SIGNAL(dataEdited(QModelIndex)),
            this,SLOT(onEditFilter(QModelIndex)));
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
