#include "propref.h"
#include <QMdiSubWindow>
#include "dbxmlstruct.h"
#include "xmldelegate.h"

PropRef::PropRef(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    connect(refWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            refItemWidget,SLOT(setRootIndex(QModelIndex)));
    connect(refWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(refWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(refWidget,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));

    connect(refWidget, SIGNAL(edited(bool)), this, SLOT(edit(bool)));
    connect(refWidget, SIGNAL(edited(bool)), refItemWidget, SLOT(edit(bool)));
    connect(toolButtonAdd,  SIGNAL(clicked()), refWidget, SLOT(add()));
    connect(toolButtonDel,  SIGNAL(clicked()), refItemWidget, SLOT(revert()));
    connect(toolButtonDel,  SIGNAL(clicked()), refWidget, SLOT(remove()));
    connect(toolButtonEdit, SIGNAL(clicked()), refWidget, SLOT(edit()));

    connect(pushButtonPropCancel, SIGNAL(clicked()), refItemWidget, SLOT(revert()));
    connect(pushButtonPropCancel, SIGNAL(clicked()), refWidget, SLOT(revert()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), refItemWidget, SLOT(submit()));
    connect(pushButtonPropSave,   SIGNAL(clicked()), refWidget, SLOT(submit()));
}

PropRef::~PropRef()
{

}

void PropRef::setModel(TreeXmlHashModel *model)
{
    refWidget->setModel(model);
    refItemWidget->setModel(model);

    m_model = model;
}

TreeXmlHashModel *PropRef::model()
{
    return m_model;
}

void PropRef::setCurrentRef(const QModelIndex &index)
{
    refWidget->setCurrent(index);
}

void PropRef::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString refItemName = modelData(DBREFXML::REF, DBREFXML::NAME,index).toString();
    QString id = modelData(DBREFXML::REF, DBREFXML::ID,index).toString();

    this->setObjectName("PropRef::" + id);
    subWindow->setWindowTitle(tr("Справочник: ")+refItemName);
}

void PropRef::closeTab(const QModelIndex &index)
{
    Q_UNUSED(index);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    subWindow->close();
}

QVariant PropRef::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                      tag,attr)).data();
}

void PropRef::edit(bool flag)
{
    if (refWidget->isEmpty()){
        toolButtonAdd->setDisabled(true);
        flag = true;
    } else
        toolButtonAdd->setEnabled(true);

    pushButtonPropSave->setEnabled(flag);
    pushButtonPropCancel->setEnabled(flag);
    toolButtonEdit->setDisabled(flag);
}