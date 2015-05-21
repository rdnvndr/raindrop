#include "entitywidget.h"
#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"
#include <QStringListModel>

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

EntityWidget::EntityWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);

    connect(comboBoxBasicUnit, SIGNAL(activated(int)),
            this, SLOT(changeUnit(int)));
}

EntityWidget::~EntityWidget()
{

}

void EntityWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    connect(model,SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),
            this,SLOT(rowsAboutToBeRemoved(QModelIndex,int,int)));

    dataMapper()->addMapping(lineEditEntityName,
                             model->columnDisplayedAttr(DBENTITYXML::ENTITY,
                                                        DBENTITYXML::NAME));
    dataMapper()->addMapping(lineEditEntityDesc,
                             model->columnDisplayedAttr(DBENTITYXML::ENTITY,
                                                        DBENTITYXML::DESCRIPTION));
    dataMapper()->addMapping(lineEditDimensionSymbol,
                             model->columnDisplayedAttr(DBENTITYXML::ENTITY,
                                                        DBENTITYXML::DIMENSIONSYMBOL));
    dataMapper()->addMapping(comboBoxBasicUnit,
                             model->columnDisplayedAttr(DBENTITYXML::ENTITY,
                                                        DBENTITYXML::BASICUNIT));

}

bool EntityWidget::isEmpty()
{
    return lineEditEntityName->text().isEmpty();
}

void EntityWidget::add()
{
    AbstractEditorWidget::add(DBENTITYXML::ENTITY);
}

void EntityWidget::edit(bool flag)
{
    if (isEmpty()) flag = true;
    groupBoxEntity->setEnabled(flag);

    emit edited(flag);
}

void EntityWidget::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(DBENTITYXML::ENTITY,
                                                     DBENTITYXML::NAME,
                                                     lineEditEntityName->text());
    QModelIndex srcIndex = model()->index(dataMapper()->currentIndex(),0,
                                          dataMapper()->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Сущность ЕИ с таким именем уже существует"));
            return;
        }
    }

    AbstractEditorWidget::submit();
}

void EntityWidget::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)

    for (int row = end; row >= start; row--) {
        if (comboBoxBasicUnit->currentIndex() == row) {
            comboBoxBasicUnit->setCurrentIndex(-1);
            return;
        }
    }
}

void EntityWidget::setUnitModel(QAbstractItemModel *model)
{
    comboBoxBasicUnit->setModel(model);
}

void EntityWidget::setUnitRootIndex(const QModelIndex &index)
{
    comboBoxBasicUnit->setRootModelIndex(index);
}

void EntityWidget::setUnitColumn(int column)
{
    comboBoxBasicUnit->setModelColumn(column);
}

void EntityWidget::changeUnit(int current)
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

    for (int row = 0; row < count; row++) {
        index = parent.child(row, columnCoeff);
        model->setData(index,QString("%1").arg(index.data().toFloat()/coeff));
        index = parent.child(row, columnDelta);
        model->setData(index,QString("%1").arg(index.data().toFloat()-delta));
    }
}

}}
