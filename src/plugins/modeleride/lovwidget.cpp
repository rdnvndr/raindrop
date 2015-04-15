#include "lovwidget.h"

#include <QMessageBox>
#include <QToolTip>

#include <metadatamodel/dbxmlstruct.h>

#include "regexpvalidator.h"

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

LovWidget::LovWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);

    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9_]{0,26}|^[A-Za-z]{0}"));
    lineEditLovName->setValidator(validator);
    connect(validator,SIGNAL(stateChanged(QValidator::State)),
            this,SLOT(validateLovName(QValidator::State)));

    m_typeAttrModel = new QStringListModel();
    const QStringList attrTypeList = (QStringList()
                                       << DBTYPEXML::BOOLEAN
                                       << DBTYPEXML::BINARY
                                       << DBTYPEXML::CHAR
                                       << DBTYPEXML::DATE
                                       << DBTYPEXML::DECIMAL
                                       << DBTYPEXML::DOUBLE
                                       << DBTYPEXML::INTEGER
                                       << DBTYPEXML::STRING
                                       << DBTYPEXML::TIME
                                       << DBTYPEXML::TIMESHTAMP
                                       );
    m_typeAttrModel->setStringList(attrTypeList);
}

LovWidget::~LovWidget()
{
    delete lineEditLovName->validator();
    delete m_typeAttrModel;
}

bool LovWidget::isEmpty()
{
    return lineEditLovName->text().isEmpty();;
}

void LovWidget::add()
{
    AbstractEditorWidget::add(DBLOVXML::LOV);
}

void LovWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    dataMapper()->addMapping(lineEditLovName,
                             model->columnDisplayedAttr(DBLOVXML::LOV,
                                                        DBLOVXML::NAME));
    dataMapper()->addMapping(lineEditLovAlias,
                             model->columnDisplayedAttr(DBLOVXML::LOV,
                                                        DBLOVXML::ALIAS));
    dataMapper()->addMapping(comboBoxLovType,
                             model->columnDisplayedAttr(DBLOVXML::LOV,
                                                        DBLOVXML::TYPE));
    comboBoxLovType->setModel(m_typeAttrModel);
}

void LovWidget::setCurrent(const QModelIndex &index)
{
    AbstractEditorWidget::setCurrent(index);

    int indexType = comboBoxLovType->findText(modelData(
                                                  DBLOVXML::LOV,
                                                  DBLOVXML::TYPE,
                                                  index).toString());
    comboBoxLovType->setCurrentIndex(indexType);
}

void LovWidget::edit(bool flag)
{
    if (isEmpty()) flag = true;
    groupBoxLov->setEnabled(flag);

    emit edited(flag);
}

void LovWidget::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(DBLOVXML::LOV,
                                                    DBLOVXML::NAME,
                                                    lineEditLovName->text());
    QModelIndex srcIndex = model()->index(dataMapper()->currentIndex(),0,
                                          dataMapper()->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Список значений с таким именем уже существует"));
            return;
        }
    }

    AbstractEditorWidget::submit();
}

void LovWidget::validateLovName(QValidator::State state) const
{
    if(state != QValidator::Acceptable)
        QToolTip::showText(lineEditLovName->mapToGlobal(QPoint(0,5)),
                           tr("Имя списка значений должно содержать только латинские\n"
                              "символы и цифры длиной не более 27 символов"));
    else
        QToolTip::hideText();
}

}}
