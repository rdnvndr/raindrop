#include "classwidget.h"

#include <QToolTip>

#include <metadatamodel/dbxmlstruct.h>
#include <metadatamodel/classmodelxml.h>

#include "xmldelegate.h"
#include "regexpvalidator.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

ClassWidget::ClassWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);

    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9_]{0,26}|^[A-Za-z]{0}"));
    lineEditClassName->setValidator(validator);
    connect(validator,SIGNAL(stateChanged(QValidator::State)),
            this,SLOT(validateClassName(QValidator::State)));

    m_typeClassModel = new QStringListModel();
    const QStringList classType = (QStringList()
                                   << DBCLASSTYPEXML::STANDART
                                   << DBCLASSTYPEXML::SYSTEM
                                   << DBCLASSTYPEXML::USER
                                   );
    m_typeClassModel->setStringList(classType);

    m_viewClassModel = new QStringListModel();
    const QStringList classView = (QStringList()
                                   << DBCLASSVIEWXML::ABSTRACT
                                   << DBCLASSVIEWXML::CONTEXT
                                   << DBCLASSVIEWXML::EMBEDDED
                                   << DBCLASSVIEWXML::NORMAL
                                   );
    m_viewClassModel->setStringList(classView);

    lineEditClassParent->setReadOnly(true);

    connect(toolButtonAddClass,SIGNAL(clicked()),this,SLOT(add()));
    connect(toolButtonDelClass,SIGNAL(clicked()),this,SLOT(remove()));
    connect(pushButtonPropSave,SIGNAL(clicked()),this,SLOT(submit()));
    connect(pushButtonPropCancel,SIGNAL(clicked()),this,SLOT(revert()));
    connect(toolButtonEditClass,SIGNAL(clicked()),this,SLOT(edit()));
}

ClassWidget::~ClassWidget()
{
    delete lineEditClassName->validator();
    delete m_typeClassModel;
    delete m_viewClassModel;
}

void ClassWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    comboBoxClassType->setModel(m_typeClassModel);
    classViewComboBox->setModel(m_viewClassModel);

    dataMapper()->addMapping(lineEditClassName,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::NAME));
    dataMapper()->addMapping(lineEditClassAlias,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::ALIAS));

    dataMapper()->addMapping(comboBoxClassType,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::TYPE));

    dataMapper()->addMapping(lineEditClassParent,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::PARENT));

    dataMapper()->addMapping(classViewComboBox,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::VIEW));
    dataMapper()->addMapping(spinBoxVersionCount,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::VERCOUNT));
    dataMapper()->addMapping(plainTextEditShowAttr,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::TEMPLATE));
    dataMapper()->addMapping(pushButtonIcon,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::ICON));
}

void ClassWidget::add()
{
    if (AbstractEditorWidget::add(DBCLASSXML::CLASS)) {
        comboBoxClassType->setCurrentIndex(2);
        classViewComboBox->setCurrentIndex(3);
    }
}

void ClassWidget::setCurrent(const QModelIndex &index)
{
    AbstractEditorWidget::setCurrent(index);
    int indexType = comboBoxClassType->findText(modelData(DBCLASSXML::CLASS,
                                                          DBCLASSXML::TYPE,
                                                          index).toString());
    comboBoxClassType->setCurrentIndex(indexType);

    int indexView = classViewComboBox->findText(modelData(DBCLASSXML::CLASS,
                                                          DBCLASSXML::VIEW,
                                                          index).toString());
    classViewComboBox->setCurrentIndex(indexView);
}

bool ClassWidget::isEdit()
{
    return groupBoxClass->isEnabled();
}

void ClassWidget::edit(bool flag)
{
    if (lineEditClassName->text().isEmpty()){
        toolButtonAddClass->setDisabled(true);
    } else
        toolButtonAddClass->setEnabled(true);

    groupBoxClass->setEnabled(flag);
    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEditClass->setDisabled(flag);
}

bool ClassWidget::isEmpty()
{
    return lineEditClassName->text().isEmpty();
}

void ClassWidget::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(DBCLASSXML::CLASS,
                                                     DBCLASSXML::NAME,
                                                     lineEditClassName->text());
    QModelIndex srcIndex = model()->index(dataMapper()->currentIndex(),0,
                                          dataMapper()->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Класс с таким именем уже существует"));
            return;
        }
    }

    AbstractEditorWidget::submit();
}

void ClassWidget::validateClassName(QValidator::State state) const
{

    if(state != QValidator::Acceptable)
        QToolTip::showText(lineEditClassName->mapToGlobal(QPoint(0,5)),
                           tr("Имя класса должно содержать только латинские\n"
                              "символы и цифры длиной не более 27 символов"));
    else
        QToolTip::hideText();
}

}}
