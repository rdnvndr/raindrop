#include "propclass.h"
#include "dbxmlstruct.h"
#include <treexmlmodel/xmldelegate.h>

PropClass::PropClass(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    connect(classWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            attrWidget,SLOT(setRootIndex(QModelIndex)));
    connect(classWidget,SIGNAL(currentIndexChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(classWidget,SIGNAL(dataChanged(QModelIndex)),
            this,SLOT(setTabName(QModelIndex)));
    connect(classWidget,SIGNAL(dataRemoved(QModelIndex)),
            this,SLOT(closeTab(QModelIndex)));
}

PropClass::~PropClass()
{

}

void PropClass::setModel(TreeXMLModel *model)
{
    classWidget->setModel(model);
    attrWidget->setModel(model);
    m_model = model;
}

void PropClass::setCurrentClass(QModelIndex index)
{
    classWidget->setCurrent(index);
}

void PropClass::setTabName(const QModelIndex &index){
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());

    QString className = modelData(DBATTRXML::ATTR, DBATTRXML::NAME,index).toString();

    this->setObjectName("PropClass::" + className);
    subWindow->setWindowTitle(tr("Класс: ")+className);
}

void PropClass::closeTab(const QModelIndex &index)
{
    Q_UNUSED(index);

    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *> (this->parent());
    subWindow->close();
}

QVariant PropClass::modelData(QString typeName, QString attr, const QModelIndex& index)
{
    return index.sibling(index.row(), m_model->indexDisplayedAttr(
                      typeName,attr)).data();
}

TreeXMLModel *PropClass::model()
{
    return m_model;
}
