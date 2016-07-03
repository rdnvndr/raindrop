#include "numeratorrangewidget.h"

#include <metadatamodel/dbxmlstruct.h>
#include <QMessageBox>

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

NumeratorRangeWidget::NumeratorRangeWidget(QWidget *parent) :
    AbstractModifyWidget(parent)
{
    setupUi(this);

    connect(addToolButton,SIGNAL(clicked()),this,SLOT(add()));
    connect(deleteToolButton,SIGNAL(clicked()),this,SLOT(remove()));

    AbstractModifyWidget::setItemView(numeratorRangeTableView);
//    itemView()->setItemDelegate(new LovDelegate());
}

NumeratorRangeWidget::~NumeratorRangeWidget()
{
//     delete itemView()->itemDelegate();
}

void NumeratorRangeWidget::setModel(TreeXmlHashModel *model)
{
    AbstractModifyWidget::setModel(model);

    proxyModel()->setHeaderData(0,  Qt::Horizontal, tr("Регулярное выражение"));
    proxyModel()->setHeaderData(1,  Qt::Horizontal, tr("Список"));
    proxyModel()->setHeaderData(2,  Qt::Horizontal, tr("Идентификатор"));

    for (int column = 2; column < 18; ++column)
        numeratorRangeTableView->setColumnHidden(column,true);

    numeratorRangeTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    numeratorRangeTableView->horizontalHeader()->setDefaultSectionSize(200);
}

void NumeratorRangeWidget::add()
{
    AbstractModifyWidget::add(DBNUMERATORRANGEXML::NUMERATORRANGE);
}

void NumeratorRangeWidget::edit(bool flag)
{
    AbstractModifyWidget::edit(flag);
    addToolButton->setEnabled(flag);
    deleteToolButton->setEnabled(flag);
}

}}
