#include "numeratorwidget.h"

#include <QMessageBox>
#include <QToolTip>

#include <metadatamodel/dbxmlstruct.h>

#include "regexpvalidator.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

NumeratorWidget::NumeratorWidget(QWidget *parent) :
     AbstractEditorWidget(parent)
{
    setupUi(this);

    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9_]{0,26}|^[A-Za-z]{0}"));
    numeratorNameLineEdit->setValidator(validator);
    connect(validator, &RegExpValidator::stateChanged,
            this, &NumeratorWidget::validateNumeratorName);


    m_uniqueNumerator = new QStringListModel();
    const QStringList unique = (QStringList()
                                   << DBUNIQUENUMERATORXML::OBJECT
                                   << DBUNIQUENUMERATORXML::CLASS
                                   << DBUNIQUENUMERATORXML::COMP
                                   );
    m_uniqueNumerator->setStringList(unique);
}

NumeratorWidget::~NumeratorWidget()
{
    delete numeratorNameLineEdit->validator();
    delete m_uniqueNumerator;
}

void NumeratorWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    numeratorUniqueComboBox->setModel(m_uniqueNumerator);

    dataMapper()->addMapping(numeratorNameLineEdit,
                             model->columnDisplayedAttr(DBNUMERATORXML::NUMERATOR,
                                                        DBNUMERATORXML::NAME));
    dataMapper()->addMapping(numeratorAliasLineEdit,
                             model->columnDisplayedAttr(DBNUMERATORXML::NUMERATOR,
                                                        DBNUMERATORXML::ALIAS));
    dataMapper()->addMapping(numeratorUniqueComboBox,
                             model->columnDisplayedAttr(DBNUMERATORXML::NUMERATOR,
                                                        DBNUMERATORXML::UNIQUE));
    dataMapper()->addMapping(numeratorStepSpinBox,
                             model->columnDisplayedAttr(DBNUMERATORXML::NUMERATOR,
                                                        DBNUMERATORXML::STEP));
}

bool NumeratorWidget::isEmpty()
{
    return numeratorNameLineEdit->text().isEmpty();
}

bool NumeratorWidget::add()
{
    if (AbstractEditorWidget::add(DBNUMERATORXML::NUMERATOR)) {
        numeratorUniqueComboBox->setCurrentIndex(0);
        return true;
    }
    return false;
}

void NumeratorWidget::setCurrent(const QModelIndex &index)
{
    AbstractEditorWidget::setCurrent(index);
    qint32 uniqueIndex = numeratorUniqueComboBox->findText(
                modelData(DBNUMERATORXML::NUMERATOR, DBNUMERATORXML::UNIQUE,
                          index).toString());
    numeratorUniqueComboBox->setCurrentIndex(uniqueIndex);
}

void NumeratorWidget::edit(bool flag)
{
    if (isEmpty()) flag = true;
    numeratorGroupBox->setEnabled(flag);

    emit edited(flag);
}

void NumeratorWidget::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(DBNUMERATORXML::NUMERATOR,
                                                    DBNUMERATORXML::NAME,
                                                    numeratorNameLineEdit->text());
    QModelIndex srcIndex = model()->index(dataMapper()->currentIndex(),0,
                                          dataMapper()->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Нумератор с таким именем уже существует"));
            return;
        }
    }

    AbstractEditorWidget::submit();
}

void NumeratorWidget::validateNumeratorName(QValidator::State state) const
{
    if(state != QValidator::Acceptable)
        QToolTip::showText(numeratorNameLineEdit->mapToGlobal(QPoint(0,5)),
                           tr("Имя нумератора должно содержать только латинские\n"
                              "символы и цифры длиной не более 27 символов"));
    else
        QToolTip::hideText();
}

}}
