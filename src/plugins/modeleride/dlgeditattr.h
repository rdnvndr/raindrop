#ifndef DLGEDITATTR_H
#define DLGEDITATTR_H

#include "ui_dlgeditattr.h"
#include "QDataWidgetMapper"
#include <QStringListModel>
#include <QTreeView>

class DlgEditAttr : public QDialog, private Ui::dlgEditAttr
{
    Q_OBJECT
    
public:
    explicit DlgEditAttr(QWidget *parent = 0);
    ~DlgEditAttr();
    void setModel(QAbstractItemModel *model);
    QAbstractItemModel *model();

    void setCurrentModelIndex(QModelIndex index);

public slots:
    void setCheckArray(bool check);
    void changeType(QString s);
    void changeUpValue(double newValue);
    void changeDownValue(double newValue);

private:
    QAbstractItemModel *m_model;
    QDataWidgetMapper  *m_mapper;
    QStringListModel *m_typeModel;
};

#endif // DLGEDITATTR_H
