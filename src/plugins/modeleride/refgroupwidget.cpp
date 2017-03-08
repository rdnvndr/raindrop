#include "refgroupwidget.h"

#include <QToolTip>

#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"
#include "regexpvalidator.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

RefGroupWidget::RefGroupWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);


    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9_]{0,26}|^[A-Za-z]{0}"));
    lineEditRefGroupName->setValidator(validator);
    connect(validator, &RegExpValidator::stateChanged,
            this, &RefGroupWidget::validateRefGroupName);

    connect(toolButtonAdd, &QToolButton::clicked, this, &RefGroupWidget::add);
    connect(toolButtonDel, &QToolButton::clicked, this, &RefGroupWidget::remove);
    connect(toolButtonEdit, &QToolButton::clicked, this,
            static_cast<void (AbstractEditorWidget::*)()>(&AbstractEditorWidget::edit));

    connect(pushButtonPropCancel, &QPushButton::clicked, this, &RefGroupWidget::revert);
    connect(pushButtonPropSave, &QPushButton::clicked, this, &RefGroupWidget::submit);
}

RefGroupWidget::~RefGroupWidget()
{

}

void RefGroupWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    dataMapper()->addMapping(lineEditRefGroupName,
                             model->columnDisplayedAttr(DBREFGROUPXML::REFGROUP,
                                                        DBREFGROUPXML::NAME));
    dataMapper()->addMapping(lineEditRefGroupAlias,
                             model->columnDisplayedAttr(DBREFGROUPXML::REFGROUP,
                                                        DBREFGROUPXML::ALIAS));
}

bool RefGroupWidget::isEmpty()
{
    return lineEditRefGroupName->text().isEmpty();
}

bool RefGroupWidget::add()
{
    return AbstractEditorWidget::add(DBREFGROUPXML::REFGROUP);
}

void RefGroupWidget::edit(bool flag)
{

    if (groupBoxRefGroup->isEnabled()==flag)
        return;

    if (isEmpty()) {
        toolButtonAdd->setDisabled(true);
        flag = true;
    } else
        toolButtonAdd->setEnabled(true);

    groupBoxRefGroup->setEnabled(flag);
    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEdit->setDisabled(flag);

    emit edited(flag);
}

void RefGroupWidget::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(
                DBREFGROUPXML::REFGROUP,
                DBREFGROUPXML::NAME,
                lineEditRefGroupName->text());
    QModelIndex srcIndex = model()->index(dataMapper()->currentIndex(),0,
                                          dataMapper()->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Группа справочника с таким именем уже существует"));
            return;
        }
    }

    AbstractEditorWidget::submit();
}

void RefGroupWidget::validateRefGroupName(QValidator::State state) const
{
    if(state != QValidator::Acceptable)
        QToolTip::showText(lineEditRefGroupName->mapToGlobal(QPoint(0,5)),
                           tr("Имя группы должно содержать только латинские\n"
                              "символы и цифры длиной не более 27 символов"));
    else
        QToolTip::hideText();
}

}}
