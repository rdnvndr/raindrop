#include "refwidget.h"

#include <QMdiSubWindow>
#include <QStringListModel>

#include <metadatamodel/dbxmlstruct.h>

using namespace RTPTechGroup::MetaDataModel;
using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

RefWidget::RefWidget(QWidget *parent) :
    AbstractEditorWidget(parent)
{
    setupUi(this);
}

RefWidget::~RefWidget()
{

}

void RefWidget::setModel(TreeXmlHashModel *model)
{
    AbstractEditorWidget::setModel(model);

    dataMapper()->addMapping(lineEditName,
                             model->columnDisplayedAttr(DBREFXML::REF,
                                                        DBREFXML::NAME));
    dataMapper()->addMapping(lineEditAlias,
                             model->columnDisplayedAttr(DBREFXML::REF,
                                                        DBREFXML::ALIAS));
}

bool RefWidget::isEmpty()
{
    return lineEditName->text().isEmpty();
}

bool RefWidget::add()
{
   return AbstractEditorWidget::add(DBREFXML::REF);
}

void RefWidget::edit(bool flag)
{
    if (isEmpty()) flag = true;
    groupBox->setEnabled(flag);

    emit edited(flag);
}

void RefWidget::submit()
{
    QModelIndex existIndex = model()->indexHashAttr(DBREFXML::REF,
                                                    DBREFXML::NAME,
                                                    lineEditName->text());
    QModelIndex srcIndex = model()->index(dataMapper()->currentIndex(),0,
                                          dataMapper()->rootIndex());

    if (existIndex.isValid()){
        if (existIndex.sibling(existIndex.row(),0)!=srcIndex){
            QMessageBox::warning(this,tr("Предупреждение"),
                                 tr("Справочник с таким именем уже существует"));
            return;
        }
    }

    AbstractEditorWidget::submit();
}

}}
