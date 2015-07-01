#include "quantitywidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"
#include <QStringListModel>

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

QuantityWidget::QuantityWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);

    connect(comboBoxBasicUnit, SIGNAL(activated(int)),
            this, SLOT(changeUnit(int)));
}

QuantityWidget::~QuantityWidget()
{

}

void QuantityWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    connect(model,SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),
            this,SLOT(rowsAboutToBeRemoved(QModelIndex,int,int)));

    dataMapper()->addMapping(lineEditQuantityName,
                             model->columnDisplayedAttr(DBQUANTITYXML::QUANTITY,
                                                        DBQUANTITYXML::NAME));
    dataMapper()->addMapping(lineEditQuantityAlias,
                             model->columnDisplayedAttr(DBQUANTITYXML::QUANTITY,
                                                        DBQUANTITYXML::ALIAS));
    dataMapper()->addMapping(lineEditDimensionSymbol,
                             model->columnDisplayedAttr(DBQUANTITYXML::QUANTITY,
                                                        DBQUANTITYXML::DIMENSION));
    dataMapper()->addMapping(comboBoxBasicUnit,
                             model->columnDisplayedAttr(DBQUANTITYXML::QUANTITY,
                                                        DBQUANTITYXML::BASICUNIT));

}

bool QuantityWidget::isEmpty()
{
    return lineEditQuantityName->text().isEmpty();
}

void QuantityWidget::add()
{
    AbstractEditorWidget::add(DBQUANTITYXML::QUANTITY);
}

void QuantityWidget::edit(bool flag)
{
    if (isEmpty()) flag = true;
    groupBoxQuantity->setEnabled(flag);

    emit edited(flag);
}

void QuantityWidget::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(DBQUANTITYXML::QUANTITY,
                                                     DBQUANTITYXML::NAME,
                                                     lineEditQuantityName->text());
    QModelIndex srcIndex = model()->index(dataMapper()->currentIndex(),0,
                                          dataMapper()->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Величина с таким именем уже существует"));
            return;
        }
    }

    AbstractEditorWidget::submit();
}

void QuantityWidget::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)

    for (int row = end; row >= start; --row) {
        if (comboBoxBasicUnit->currentIndex() == row) {
            comboBoxBasicUnit->setCurrentIndex(-1);
            return;
        }
    }
}

void QuantityWidget::setUnitModel(QAbstractItemModel *model)
{
    comboBoxBasicUnit->setModel(model);
}

void QuantityWidget::setUnitRootIndex(const QModelIndex &index)
{
    comboBoxBasicUnit->setRootModelIndex(index);
}

void QuantityWidget::setUnitColumn(int column)
{
    comboBoxBasicUnit->setModelColumn(column);
}

void QuantityWidget::changeUnit(int current)
{
    Q_UNUSED(current)

    // Для перевода ЕИ необходимо разделить на текущий koeff и отнять delta
    // т.е. x/koeff - delta

    QModelIndex index  = comboBoxBasicUnit->view()->currentIndex();
    QModelIndex parent = comboBoxBasicUnit->rootModelIndex();
    QAbstractItemModel *model = comboBoxBasicUnit->model();

    int count  = parent.model()->rowCount(parent);
    int columnCoeff = this->model()->columnDisplayedAttr(
                DBUNITXML::UNIT,
                DBUNITXML::COEFF);
    int columnDelta = this->model()->columnDisplayedAttr(
                DBUNITXML::UNIT,
                DBUNITXML::DELTA);

    float coeff = index.sibling(index.row(),columnCoeff).data().toFloat();
    float delta = index.sibling(index.row(),columnDelta).data().toFloat();
    coeff = (coeff==0)? 1: coeff;

    for (int row = 0; row < count; ++row) {
        index = parent.child(row, columnCoeff);
        model->setData(index,QString("%1").arg(index.data().toFloat()/coeff));
        index = parent.child(row, columnDelta);
        model->setData(index,QString("%1").arg(index.data().toFloat()-delta));
    }
}

}}
