#ifndef PROPCLASS_H
#define PROPCLASS_H

#include "ui_propclass.h"
#include <treexmlmodel/treexmlmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <QDataWidgetMapper>
#include <QStringListModel>

class PropClass : public QWidget, public Ui::PropClass
{
    Q_OBJECT
    
public:
    explicit PropClass(QWidget *parent = 0);
    virtual ~PropClass();
    void setModel(TreeXMLModel *model);
    void setCurrentModelIndex(QModelIndex index);
    TreeXMLModel* model();    



public slots:
    void addPropClass();
    void addPropAttr();
    void showPropAttr();
    void removePropAttr();
    void removePropClass();
    void setCurrentModelIndexAttr(QModelIndex index);
private:
    TreeXMLModel* m_model;
    QDataWidgetMapper* m_mapper;
    QDataWidgetMapper* m_mapperAttr;
    TableXMLProxyModel* m_attrModel;
    QStringListModel *m_typeAttrModel;
    QStringListModel *m_typeClassModel;
};

#endif // PROPCLASS_H
