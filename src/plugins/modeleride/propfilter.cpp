#include <QMdiSubWindow>
#include "propfilter.h"
#include <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

PropFilter::PropFilter(QWidget *parent) :
    AbstractPropEditor(parent)
{
    setupUi(this);

    connect(propWidget, &AbstractEditorWidget::currentIndexChanged,
            this, &PropFilter::setTabName);
    connect(propWidget, &AbstractEditorWidget::dataChanged,
            this, &PropFilter::setTabName);
    connect(propWidget, &AbstractEditorWidget::dataAboutToBeRemoved,
            this, &AbstractPropEditor::closeTab);
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

QString PropFilter::dataId(const QModelIndex &index)
{
    return modelData(DBFILTERXML::FILTER, DBFILTERXML::ID,index).toString();
}

void PropFilter::setCurrent(const QModelIndex &index)
{
    propWidget->setCurrent(index);
}

void PropFilter::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString className = modelData(DBFILTERXML::FILTER, DBFILTERXML::NAME,index).toString();
    QString id = this->dataId(index);

    this->setObjectName("PropFilter::" + id);
    if (subWindow) {
        subWindow->setWindowIcon(qvariant_cast<QIcon>(index.data(Qt::DecorationRole)));
        subWindow->setWindowTitle(className);
    }
}

void PropFilter::edit(bool flag)
{
    propWidget->edit(flag);
}

}}
