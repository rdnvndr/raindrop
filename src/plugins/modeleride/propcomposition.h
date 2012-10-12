#ifndef PROPCOMPOSITION_H
#define PROPCOMPOSITION_H

#include "ui_propcomposition.h"
#include <treexmlmodel/treexmlmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <QDataWidgetMapper>
#include <QStringListModel>

class PropComposition : public QWidget, private Ui::PropComposition
{
    Q_OBJECT
    
public:
    explicit PropComposition(QWidget *parent = 0);
    void setModel(TreeXMLModel *model);
    TreeXMLModel* model();

public slots:
    void setCurrentClass(QModelIndex index);
    void setTabName(const QModelIndex &index);
    void closeTab(const QModelIndex &index = QModelIndex());

private:
    QVariant modelData(QString typeName, QString attr, const QModelIndex &index);
    TreeXMLModel* m_model;
};

#endif // PROPCOMPOSITION_H
