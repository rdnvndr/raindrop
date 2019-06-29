#include "quantitywidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

#include <QStringListModel>
#include <QAbstractItemView>

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

QuantityWidget::QuantityWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);

    connect(comboBoxBasicUnit,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
            this, &QuantityWidget::changeUnit);
}

QuantityWidget::~QuantityWidget()
{

}

void QuantityWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    connect(model, &QAbstractItemModel::rowsAboutToBeRemoved,
            this, &QuantityWidget::rowsAboutToBeRemoved);

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

bool QuantityWidget::add()
{
    return AbstractEditorWidget::add(DBQUANTITYXML::QUANTITY);
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

void QuantityWidget::rowsAboutToBeRemoved(const QModelIndex &parent, qint32 start, qint32 end)
{
    Q_UNUSED(parent)

    for (qint32 row = end; row >= start; --row) {
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

void QuantityWidget::setUnitColumn(qint32 column)
{
    comboBoxBasicUnit->setModelColumn(column);
}

void QuantityWidget::changeUnit(qint32 current)
{
    Q_UNUSED(current)

    // Для перевода ЕИ необходимо разделить на текущий koeff и отнять delta
    // т.е. x/koeff - delta

    QModelIndex index  = comboBoxBasicUnit->view()->currentIndex();
    QModelIndex parent = comboBoxBasicUnit->rootModelIndex();
    QAbstractItemModel *model = comboBoxBasicUnit->model();

    qint32 count  = parent.model()->rowCount(parent);
    qint32 columnCoeff = this->model()->columnDisplayedAttr(
                DBUNITXML::UNIT,
                DBUNITXML::COEFF);
    qint32 columnDelta = this->model()->columnDisplayedAttr(
                DBUNITXML::UNIT,
                DBUNITXML::DELTA);

    qreal coeff = index.sibling(index.row(),columnCoeff).data().toFloat();
    qreal delta = index.sibling(index.row(),columnDelta).data().toFloat();
    coeff = (coeff==0)? 1: coeff;

    for (qint32 row = 0; row < count; ++row) {
        index = childIdx(row, columnCoeff,parent);
        model->setData(index,QString("%1").arg(index.data().toFloat()/coeff));
        index = childIdx(row, columnDelta,parent);
        model->setData(index,QString("%1").arg(index.data().toFloat()-delta));
    }
}

}}
