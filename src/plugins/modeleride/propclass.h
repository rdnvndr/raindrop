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
    void setCurrentClass(QModelIndex index);
    void setTabName(const QModelIndex &index);
    void closeTab(const QModelIndex &index);
private:
    QVariant modelData(QString typeName, QString attr, const QModelIndex &index);
    TreeXMLModel* m_model;
};

#endif // PROPCLASS_H
