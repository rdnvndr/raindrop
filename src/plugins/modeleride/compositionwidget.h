#ifndef COMPOSITIONWIDGET_H
#define COMPOSITIONWIDGET_H

#include "ui_compositionwidget.h"
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlmodel.h>
#include <QDataWidgetMapper>

class CompositionWidget : public QWidget, private Ui::CompositionWidget
{
    Q_OBJECT
    
public:
    explicit CompositionWidget(QWidget *parent = 0);
    virtual ~CompositionWidget();
    void setModel(TreeXMLModel *model);


public slots:
    void add();
    void remove();
    void setCurrent(QModelIndex index);
    void edit();
    void showParent(bool flag);
    void setRootIndex(QModelIndex index);

signals:
    void dataAdded(QModelIndex index);
    void dataEdited(QModelIndex index);
    void dataChanged(QModelIndex index);
    void dataRemoved(QModelIndex index);
    void currentIndexChanged(QModelIndex index);

private:
    QVariant modelData(QString typeName, QString attr, const QModelIndex &index);
    TableXMLProxyModel* m_compositionModel;
    TreeXMLModel* m_model;
};

#endif // COMPOSITIONWIDGET_H
