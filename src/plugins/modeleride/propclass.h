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
    TreeXMLModel* model();    


public slots:
    void addClass();
    void addAttr();
    void showPropAttr();
    void removeAttr();
    void removeClass();
    void setCurrentAttr(QModelIndex index);
    void setCurrentClass(QModelIndex index);

    void changeType(QString s);
private:
    TreeXMLModel* m_model;
    QDataWidgetMapper* m_mapper;
    QDataWidgetMapper* m_mapperAttr;
    TableXMLProxyModel* m_attrModel;
    QStringListModel *m_typeAttrModel;
    QStringListModel *m_typeClassModel;

};

#endif // PROPCLASS_H
