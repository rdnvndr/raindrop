#ifndef PROPCLASS_H
#define PROPCLASS_H

#include "ui_propclass.h"
#include <treexmlmodel/treexmlmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <QDataWidgetMapper>

class PropClass : public QWidget, public Ui::PropClass
{
    Q_OBJECT
    
public:
    explicit PropClass(QWidget *parent = 0);
    void setModel(TreeXMLModel *model);
    void setCurrentModelIndex(QModelIndex index);
    TreeXMLModel* model();


public slots:
    void addPropAttr();
    void showPropAttr();
    void removePropAttr();
private:
    TreeXMLModel* m_model;
    QDataWidgetMapper* m_mapper;
    TableXMLProxyModel* m_attrModel;
};

#endif // PROPCLASS_H
