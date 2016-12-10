#include "numeratorrangewidget.h"
#include "numeratorrangedelegate.h"

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

    connect(addLovToolButton, &QToolButton::clicked,
            this, &NumeratorRangeWidget::addLov);
    connect(addRegExToolButton, &QToolButton::clicked,
            this, &NumeratorRangeWidget::addRegEx);
    connect(deleteToolButton, &QToolButton::clicked,
            this, &NumeratorRangeWidget::remove);

    connect(upRangeToolButton, &QToolButton::clicked,
            this, &NumeratorRangeWidget::up);
    connect(downRangeToolButton, &QToolButton::clicked,
            this, &NumeratorRangeWidget::down);

    AbstractModifyWidget::setItemView(numeratorRangeTableView);
    itemView()->setItemDelegate(new NumeratorRangeDelegate());
}

NumeratorRangeWidget::~NumeratorRangeWidget()
{
    delete itemView()->itemDelegate();
}

void NumeratorRangeWidget::setModel(TreeXmlHashModel *model)
{
    AbstractModifyWidget::setModel(model);

    proxyModel()->setHeaderData(0,  Qt::Horizontal, tr("Список/Регулярное выражение"));
    proxyModel()->setHeaderData(1,  Qt::Horizontal, tr("Идентификатор"));

    for (qint32 column = 1; column < 18; ++column)
        numeratorRangeTableView->setColumnHidden(column,true);

    numeratorRangeTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    numeratorRangeTableView->horizontalHeader()->setDefaultSectionSize(200);
}

void NumeratorRangeWidget::addLov()
{
    AbstractModifyWidget::add(DBNUMERATORLOVXML::NUMERATORLOV);
}

void NumeratorRangeWidget::addRegEx()
{
    AbstractModifyWidget::add(DBNUMERATORREGEXXML::NUMERATORREGEX);
}

void NumeratorRangeWidget::edit(bool flag)
{
    AbstractModifyWidget::edit(flag);
    addLovToolButton->setEnabled(flag);
    addRegExToolButton->setEnabled(flag);
    deleteToolButton->setEnabled(flag);
    upRangeToolButton->setEnabled(flag);
    downRangeToolButton->setEnabled(flag);
}

}}
