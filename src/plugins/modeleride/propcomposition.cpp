#include <QMdiSubWindow>
#include "propcomposition.h"
#include "dbxmlstruct.h"

PropComposition::PropComposition(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);   

    connect(propWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            attrWidget,SLOT(setRootIndex(QModelIndex)));
    connect(propWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(propWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(propWidget,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));
}

PropComposition::~PropComposition()
{
    if (attrWidget->isEdit()) attrWidget->revert();
    if (propWidget->isEdit()) propWidget->revert();
}

void PropComposition::setModel(TreeXmlHashModel *model)
{
    propWidget->setModel(model);
    attrWidget->setModel(model);
    m_model = model;
}

void PropComposition::setCurrentClass(const QModelIndex &index)
{
    propWidget->setCurrent(index);
}

void PropComposition::setTabName(const QModelIndex &index){
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString className = modelData(DBCOMPXML::COMP, DBCOMPXML::NAME,index).toString();
    QString id = modelData(DBCOMPXML::COMP, DBCOMPXML::ID,index).toString();

    this->setObjectName("PropComposition::" + id);
    if (subWindow)
        subWindow->setWindowTitle(tr("Состав: ")+className);
}

void PropComposition::closeTab(const QModelIndex &index)
{
    Q_UNUSED(index);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    if (subWindow)
        subWindow->close();
}

QVariant PropComposition::modelData(const QString &tag, const QString &attr, const QModelIndex& index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                      tag,attr)).data();
}

TreeXmlHashModel *PropComposition::model()
{
    return m_model;
}
