#ifndef CONDITIONDELEGATE_H
#define CONDITIONDELEGATE_H

#include "xmldelegate.h"
#include <treexmlmodel/tablexmlproxymodel.h>

class ConditionDelegate : public XmlDelegate
{
    Q_OBJECT
public:
    explicit ConditionDelegate(QObject *parent = 0);
    virtual ~ConditionDelegate();

    //! Предоставление редактора
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    //! Запись данных в модель
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    //! Делегат должен предоставить функцию копирования данных модели в редактор.
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setFirstIndex(const QModelIndex& index);
    QModelIndex firstIndex() const;
    void setSecondIndex(const QModelIndex& index);
    QModelIndex secondIndex() const;
    
private:
    QModelIndex m_firstIndex;
    QModelIndex m_secondIndex;
    TableXMLProxyModel *m_attrModel;
};

#endif // CONDITIONDELEGATE_H
