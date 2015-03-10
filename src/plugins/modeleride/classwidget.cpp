#include "classwidget.h"

#include <QToolTip>

#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"
#include "regexpvalidator.h"

using namespace RTPTechGroup::MetaDataModel;

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
}

void ClassWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    comboBoxClassType->setModel(m_typeClassModel);

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

    dataMapper()->addMapping(checkBoxAbsClass,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::ISABSTARCT));
    dataMapper()->addMapping(checkBoxActiveClass,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::ISACTIVE));

    dataMapper()->addMapping(checkBoxContextClass,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::ISCONTEXT));

    dataMapper()->addMapping(plainTextEditShowAttr,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::TEMPLATE));
    dataMapper()->addMapping(pushButtonIcon,
                             model->columnDisplayedAttr(DBCLASSXML::CLASS,
                                                        DBCLASSXML::ICON));
}

void ClassWidget::add()
{
    if (AbstractEditorWidget::add(DBCLASSXML::CLASS))
        comboBoxClassType->setCurrentIndex(0);
}

// Метод совпадает с bool ModelerIDEPlug::isRemoveClass(const QModelIndex &srcIndex)
bool ClassWidget::isRemove(const QModelIndex &srcIndex)
{
    const TreeXmlHashModel *model = dynamic_cast<const TreeXmlHashModel *>(srcIndex.model());
    if (!model)
        return false;

    bool success = true;
    QString msg;

    QString tag = srcIndex.data(TreeXmlModel::TagRole).toString();
    QStringList tags;
    tags << tag;
    if (model->rowCount(srcIndex,tags)) {
        msg += tr("Необходимо удалить классы-потомки.\n\n");
        if (success)
            success = false;
    }

    QString fieldId = model->uuidAttr(tag);
    if (fieldId.isEmpty())
        return true;

    QString guid =  srcIndex.sibling(srcIndex.row(),
                                     model->columnDisplayedAttr(
                                         tag,fieldId))
            .data().toString();

    foreach (TreeXmlHashModel::TagWithAttr tagWithAttr,
             model->fromRelation(tag))
    {
        int number = 0;

        QModelIndex linkIndex = model->indexHashAttr(
                    tagWithAttr.tag,
                    tagWithAttr.attr,
                    guid,
                    number
                    );

        while (linkIndex.isValid()) {
            QModelIndex linkParent = linkIndex.parent();
            if (linkParent.sibling(linkIndex.parent().row(),0)!= srcIndex){
                QString parentName;
                QString name;
                if (linkIndex.data(TreeXmlModel::TagRole) == DBCLASSXML::CLASS) {
                    name = tr("класс ")
                            + linkIndex.sibling(linkIndex.row(),
                                                model->columnDisplayedAttr(
                                                    DBCLASSXML::CLASS,
                                                    DBCLASSXML::NAME)
                                                ).data().toString();
                } else {
                    if (linkParent.data(TreeXmlModel::TagRole) == DBCOMPXML::COMP)
                        parentName = tr(" принадлежащий составу ")
                                + linkParent.sibling(
                                    linkParent.row(),
                                    model->columnDisplayedAttr(
                                        DBCOMPXML::COMP,
                                        DBCOMPXML::NAME)
                                    ).data().toString();
                    else
                        parentName = tr(" принадлежащий классу ")
                                + linkParent.sibling(
                                    linkParent.row(),
                                    model->columnDisplayedAttr(
                                        DBCLASSXML::CLASS,
                                        DBCLASSXML::NAME)
                                    ).data().toString();

                    if  (linkIndex.data(TreeXmlModel::TagRole) == DBCOMPXML::COMP)
                        name = tr("состав ")
                                + linkIndex.sibling(linkIndex.row(),
                                                    model->columnDisplayedAttr(
                                                        DBCOMPXML::COMP,
                                                        DBCOMPXML::NAME)
                                                    ).data().toString();
                    else
                        name = tr("атрибут ")
                                + linkIndex.sibling(linkIndex.row(),
                                                    model->columnDisplayedAttr(
                                                        DBATTRXML::ATTR,
                                                        DBATTRXML::NAME)
                                                    ).data().toString();

                }
                msg += QString(tr("Необходимо удалить %1%2.\n\n")).
                        arg(name).arg(parentName);
                if (success)
                    success = false;
            }
            number++;
            linkIndex = model->indexHashAttr(
                        tagWithAttr.tag,
                        tagWithAttr.attr,
                        guid,
                        number
                        );
        }
    }
    if (!success) {
        QMessageBox msgBox;
        msgBox.setText(tr("Удаление данного объекта не воможно."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setDetailedText(msg);
        msgBox.setWindowTitle(tr("Предупреждение"));
        msgBox.exec();
    }
    return success;
}

void ClassWidget::setCurrent(const QModelIndex &index)
{
    AbstractEditorWidget::setCurrent(index);
    int indexType = comboBoxClassType->findText(modelData(DBCLASSXML::CLASS,
                                                          DBCLASSXML::TYPE,
                                                          index).toString());
    comboBoxClassType->setCurrentIndex(indexType);
}

bool ClassWidget::isEdit()
{
    return groupBoxClass->isEnabled();
}

void ClassWidget::edit(bool flag)
{
    if (isEdit()==flag)
        return;

    if (lineEditClassName->text().isEmpty()){
        toolButtonAddClass->setDisabled(true);
        flag = true;
    }else
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
