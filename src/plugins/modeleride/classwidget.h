#ifndef CLASSWIDGET_H
#define CLASSWIDGET_H

#include "ui_classwidget.h"
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlmodel.h>

class ClassWidget : public QWidget, private Ui::ClassWidget
{
    Q_OBJECT
    
public:
    explicit ClassWidget(QWidget *parent = 0);
    virtual ~ClassWidget();
    void setModel(TreeXMLModel *model);
public slots:
    void add();
    void remove();
    void removeEmpty();
    void setCurrent(QModelIndex index);
    void edit(bool flag = true);
    void submit();
    void revert();
signals:
    void dataChanged(QModelIndex index);
    void currentIndexChanged(QModelIndex index);

private:
    QVariant modelData(QString typeName, QString attr, const QModelIndex &index);
    TreeXMLModel* m_model;
    QDataWidgetMapper* m_mapper;
    QStringListModel *m_typeClassModel;
};

#endif // CLASSWIDGET_H
