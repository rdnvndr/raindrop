#include "propentity.h"
#include <QMdiSubWindow>
#include "dbxmlstruct.h"
#include "xmldelegate.h"

PropEntity::PropEntity(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    connect(msrEntityWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            msrUnitWidget,SLOT(setRootIndex(QModelIndex)));
    connect(msrEntityWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(msrEntityWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(msrEntityWidget,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));

    connect(msrEntityWidget, SIGNAL(edited(bool)), this, SLOT(edit(bool)));
    connect(msrEntityWidget, SIGNAL(edited(bool)), msrUnitWidget, SLOT(edit(bool)));

    connect(toolButtonAddEntity,  SIGNAL(clicked()), msrEntityWidget, SLOT(add()));
    connect(toolButtonDelEntity,  SIGNAL(clicked()), msrEntityWidget, SLOT(remove()));
    connect(toolButtonEditEntity, SIGNAL(clicked()), msrEntityWidget, SLOT(edit()));

    connect(pushButtonPropSave,   SIGNAL(clicked()), msrEntityWidget, SLOT(submit()));
    connect(pushButtonPropCancel, SIGNAL(clicked()), msrEntityWidget, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), msrUnitWidget, SLOT(submit()));
    connect(pushButtonPropCancel, SIGNAL(clicked()), msrUnitWidget, SLOT(revert()));
}

PropEntity::~PropEntity()
{

}

void PropEntity::setModel(TreeXmlHashModel *model)
{
    msrEntityWidget->setModel(model);
    msrUnitWidget->setModel(model);
    m_model = model;
}

TreeXmlHashModel *PropEntity::model()
{
    return m_model;
}

void PropEntity::setCurrentEntity(const QModelIndex &index)
{
    msrEntityWidget->setCurrent(index);
}

void PropEntity::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString entityName = modelData(DBMSRENTITYXML::ENTITY, DBMSRENTITYXML::NAME,index).toString();
    QString id = modelData(DBMSRENTITYXML::ENTITY, DBMSRENTITYXML::ID,index).toString();

    this->setObjectName("PropEntity::" + id);
    subWindow->setWindowTitle(tr("Сущность ЕИ: ")+entityName);
}

void PropEntity::closeTab(const QModelIndex &index)
{
    Q_UNUSED(index);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    subWindow->close();
}

QVariant PropEntity::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                      tag,attr)).data();
}

void PropEntity::edit(bool flag)
{
    if (msrEntityWidget->isEmpty()){
        toolButtonAddEntity->setDisabled(true);
        flag = true;
    } else
        toolButtonAddEntity->setEnabled(true);

    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEditEntity->setDisabled(flag);
}
