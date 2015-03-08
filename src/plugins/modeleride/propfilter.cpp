#include <QMdiSubWindow>
#include "propfilter.h"
#include <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropFilter::PropFilter(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);

    connect(propWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(propWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(propWidget,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));
}

PropFilter::~PropFilter()
{
    if (propWidget->isEdit()) propWidget->revert();
}

void PropFilter::setModel(TreeXmlHashModel *model)
{
    propWidget->setModel(model);
    AbstractPropEditor::setModel(model);
}

void PropFilter::setCurrent(const QModelIndex &index)
{
    propWidget->setCurrent(index);
}

void PropFilter::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString className = modelData(DBFILTERXML::FILTER, DBFILTERXML::NAME,index).toString();
    QString id = modelData(DBFILTERXML::FILTER, DBFILTERXML::ID,index).toString();

    this->setObjectName("PropFilter::" + id);
    if (subWindow) {
        subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
        subWindow->setWindowTitle(className);
    }
}

void PropFilter::edit(bool flag)
{
    Q_UNUSED(flag)
}

}}
