#include <QMdiSubWindow>
#include "propfilter.h"
#include "dbxmlstruct.h"

PropFilter::PropFilter(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    /*connect(propWidget,SIGNAL(currentIndexChanged(QModelInex)),
            attrWidget,SLOT(setRootIndex(QModelIndex)));*/
    connect(propWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(propWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(propWidget,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));
}

void PropFilter::setModel(TreeXMLModel *model)
{
    propWidget->setModel(model);
    // attrWidget->setModel(model);
    m_model = model;

}

TreeXMLModel *PropFilter::model()
{
    return m_model;
}

void PropFilter::setCurrentClass(const QModelIndex &index)
{
    propWidget->setCurrent(index);
}

void PropFilter::setTabName(const QModelIndex &index)
{
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString className = modelData(DBFILTERXML::FILTER, DBFILTERXML::NAME,index).toString();
    QString id = modelData(DBFILTERXML::FILTER, DBFILTERXML::ID,index).toString();

    this->setObjectName("PropFilter::" + id);
    subWindow->setWindowTitle(tr("Фильтр: ")+className);
}

void PropFilter::closeTab(const QModelIndex &index)
{
    Q_UNUSED(index);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    subWindow->close();
}

QVariant PropFilter::modelData(const QString &tag, const QString &attr, const QModelIndex &index)
{
    return index.sibling(index.row(), m_model->columnDisplayedAttr(
                      tag,attr)).data();
}
