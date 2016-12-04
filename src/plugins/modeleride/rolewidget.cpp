#include "rolewidget.h"

#include <QMessageBox>
#include <QToolTip>

#include <metadatamodel/dbxmlstruct.h>

#include "regexpvalidator.h"

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

RoleWidget::RoleWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);

    RegExpValidator *validator =
            new RegExpValidator(QRegExp("^[A-Za-z]{1}[A-Za-z0-9_]{0,26}|^[A-Za-z]{0}"));
    lineEditRoleName->setValidator(validator);
    connect(validator, &RegExpValidator::stateChanged,
            this, &RoleWidget::validateRoleName);
}

RoleWidget::~RoleWidget()
{
    delete lineEditRoleName->validator();
}

void RoleWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    dataMapper()->addMapping(lineEditRoleName,
                             model->columnDisplayedAttr(DBROLEXML::ROLE,
                                                        DBROLEXML::NAME));

    dataMapper()->addMapping(lineEditRoleAlias,
                             model->columnDisplayedAttr(DBROLEXML::ROLE,
                                                        DBROLEXML::ALIAS));

    dataMapper()->addMapping(lineEditRoleDesc,
                             model->columnDisplayedAttr(DBROLEXML::ROLE,
                                                        DBROLEXML::DESCRIPTION));
}

bool RoleWidget::isEmpty()
{
    return lineEditRoleName->text().isEmpty();
}

void RoleWidget::add()
{
    AbstractEditorWidget::add(DBROLEXML::ROLE);
}

void RoleWidget::edit(bool flag)
{
    if (isEmpty()) flag = true;
    groupBoxRole->setEnabled(flag);

    emit edited(flag);
}

void RoleWidget::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(
                DBREFGROUPXML::REFGROUP,
                DBREFGROUPXML::NAME,
                lineEditRoleName->text());
    QModelIndex srcIndex = model()->index(dataMapper()->currentIndex(),0,
                                          dataMapper()->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Роль с таким именем уже существует"));
            return;
        }
    }

    AbstractEditorWidget::submit();
}

void RoleWidget::validateRoleName(QValidator::State state) const
{
    if(state != QValidator::Acceptable)
        QToolTip::showText(lineEditRoleName->mapToGlobal(QPoint(0,5)),
                           tr("Имя роли должно содержать только латинские\n"
                              "символы и цифры длиной не более 27 символов"));
    else
        QToolTip::hideText();
}

}}
