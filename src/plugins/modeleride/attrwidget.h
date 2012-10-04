#ifndef ATTRWIDGET_H
#define ATTRWIDGET_H

#include "ui_attrwidget.h"
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlmodel.h>

class AttrWidget : public QWidget, private Ui::AttrWidget
{
    Q_OBJECT
    
public:
    explicit AttrWidget(QWidget *parent = 0);
    virtual ~AttrWidget();
    void setModel(TreeXMLModel *model);
public slots:
    void add();
    void remove();
    void setCurrent(QModelIndex index);
    void submit();
    void edit(bool flag = true);
    void revert();
    void showParentAttr(bool flag);
    void changeType(QString s);
    void setRootIndex(QModelIndex index);
private:
    QVariant modelData(QString typeName, QString attr, const QModelIndex &index);
    TableXMLProxyModel* m_attrModel;
    TreeXMLModel* m_model;
    QDataWidgetMapper* m_mapperAttr;
    QStringListModel *m_typeAttrModel;
};

#endif // ATTRWIDGET_H
