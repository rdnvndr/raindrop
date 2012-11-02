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

void PropComposition::setModel(TreeXMLModel *model)
{
    propWidget->setModel(model);
    attrWidget->setModel(model);
    m_model = model;
}

void PropComposition::setCurrentClass(QModelIndex index)
{
    propWidget->setCurrent(index);
}

void PropComposition::setTabName(const QModelIndex &index){
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString className = modelData(DBCOMPXML::COMP, DBCOMPXML::NAME,index).toString();

    this->setObjectName("PropComposition::" + className);
    subWindow->setWindowTitle(tr("Состав: ")+className);
}

void PropComposition::closeTab(const QModelIndex &index)
{
    Q_UNUSED(index);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    subWindow->close();
}

QVariant PropComposition::modelData(QString typeName, QString attr, const QModelIndex& index)
{
    return index.sibling(index.row(), m_model->indexDisplayedAttr(
                      typeName,attr)).data();
}

TreeXMLModel *PropComposition::model()
{
    return m_model;
}