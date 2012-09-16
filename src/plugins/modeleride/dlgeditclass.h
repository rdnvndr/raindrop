#ifndef DLGEDITCLASS_H
#define DLGEDITCLASS_H

#include <QDataWidgetMapper>
#include "ui_dlgeditclass.h"

class DlgEditClass : public QDialog, private Ui::DlgEditClass
{
    Q_OBJECT
    
public:
    explicit DlgEditClass(QWidget *parent = 0);
    ~DlgEditClass();
    void setModel(QAbstractItemModel *model);
    QAbstractItemModel *model();
    void setCurrentModelIndex(QModelIndex index);

private:
    QAbstractItemModel *m_model;
    QDataWidgetMapper  *m_mapper;
};

#endif // DLGEDITCLASS_H
