#include <QMdiSubWindow>
#include <metadatamodel/dbxmlstruct.h>
#include "propclass.h"
#include "xmldelegate.h"

PropClass::PropClass(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    connect(classWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            attrWidget,SLOT(setRootIndex(QModelIndex)));
    connect(classWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            compWidget,SLOT(setRootIndex(QModelIndex)));
    connect(classWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            filterWidget,SLOT(setRootIndex(QModelIndex)));
    connect(classWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(classWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(classWidget,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));
    connect(filterWidget,SIGNAL(dataEdited(QModelIndex)),
            this,SLOT(onEditFilter(QModelIndex)));
}

PropClass::~PropClass()
{
    if (classWidget->isEdit()) classWidget->revert();
    if (attrWidget->isEdit())  attrWidget->revert();
}

void PropClass::setModel(TreeXmlHashModel *model)
{
    classWidget->setModel(model);
    attrWidget->setModel(model);
    compWidget->setModel(model);
    filterWidget->setModel(model);
    m_model = model;
}

void PropClass::setCurrentClass(const QModelIndex &index)
{
    classWidget->setCurrent(index);
}

void PropClass::setTabName(const QModelIndex &index){
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    if (!subWindow) return;

    QString className = modelData(DBCLASSXML::CLASS, DBCLASSXML::NAME,index).toString();
    QString id = modelData(DBCLASSXML::CLASS, DBCLASSXML::ID,index).toString();

    this->setObjectName("PropClass::" + id);
    subWindow->setWindowTitle(tr("Класс: ")+className);
}

void PropClass::closeTab(const QModelIndex &index)
{
    Q_UNUSED(index);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    if (subWindow)
        subWindow->close();
}

void PropClass::onEditComposition(const QModelIndex &index)
{
    emit editComposition(index);
}

QVariant PropClass::modelData(const QString &tag, const QString &attr, const QModelIndex& index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                      tag,attr)).data();
}

TreeXmlHashModel *PropClass::model()
{
    return m_model;
}


void PropClass::onEditFilter(const QModelIndex &index)
{
    emit editFilter(index);
}
