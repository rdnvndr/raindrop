#include "refgroupwidget.h"

#include <metadatamodel/dbxmlstruct.h>
#include "xmldelegate.h"

using namespace RTPTechGroup::MetaDataModel;

namespace RTPTechGroup {
namespace ModelerIde {

RefGroupWidget::RefGroupWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);

    connect(toolButtonAdd,  SIGNAL(clicked()), this, SLOT(add()));
    connect(toolButtonDel,  SIGNAL(clicked()), this, SLOT(remove()));
    connect(toolButtonEdit, SIGNAL(clicked()), this, SLOT(edit()));

    connect(pushButtonPropCancel, SIGNAL(clicked()), this, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), this, SLOT(submit()));
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
    dataMapper()->addMapping(lineEditRefGroupDesc,
                             model->columnDisplayedAttr(DBREFGROUPXML::REFGROUP,
                                                        DBREFGROUPXML::DESCRIPTION));
}

bool RefGroupWidget::isRemove(const QModelIndex &srcIndex)
{
    const TreeXmlHashModel *model = dynamic_cast<const TreeXmlHashModel *>(srcIndex.model());
    if (!model)
        return false;

    bool success = true;
    QString msg;

    QStringList tags;
    tags << DBREFXML::REF;
    if (model->hasChildren(srcIndex,tags)) {
        msg += tr("Необходимо удалить справочник.\n\n");
        success = false;
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

bool RefGroupWidget::isEmpty()
{
    return lineEditRefGroupName->text().isEmpty();
}

void RefGroupWidget::add()
{
    AbstractEditorWidget::add(DBREFGROUPXML::REFGROUP);
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

}}
