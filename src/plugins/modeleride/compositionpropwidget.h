#ifndef COMPOSITIONPROPWIDGET_H
#define COMPOSITIONPROPWIDGET_H

#include "ui_compositionpropwidget.h"
#include <treexmlmodel/treexmlmodel.h>

class CompositionPropWidget : public QWidget, private Ui::CompositionPropWidget
{
    Q_OBJECT
    
public:
    explicit CompositionPropWidget(QWidget *parent = 0);
    ~CompositionPropWidget();
    void setModel(TreeXMLModel *model);

public slots:
    void add();
    void remove();
    void removeEmpty();
    void setCurrent(QModelIndex index);
    void edit(bool flag = true);
    void submit();
    void revert();
    void rowsRemoved(QModelIndex index,int pos,int count);

signals:
    void dataChanged(QModelIndex index);
    void dataRemoved(QModelIndex index);
    void currentIndexChanged(QModelIndex index);

private:
    QVariant modelData(QString typeName, QString attr, const QModelIndex &index);
    TreeXMLModel* m_model;
    QDataWidgetMapper* m_mapper;
    QModelIndex m_oldIndex;
};

#endif // COMPOSITIONPROPWIDGET_H
