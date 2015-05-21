#ifndef CONDITIONDELEGATE_H
#define CONDITIONDELEGATE_H

#include <treexmlmodel/tablexmlproxymodel.h>

#include "xmldelegate.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Делегат редактирования условий фильтра
/*! Делегат предназначен для редактирования условий фильтра

*/

class ConditionDelegate : public XmlDelegate
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit ConditionDelegate(QObject *parent = 0);

    //! Деструктор класса
    virtual ~ConditionDelegate();

    //! Предоставление редактора
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    //! Запись данных в модель
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    //! Делегат должен предоставить функцию копирования данных модели в редактор.
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    //! Устанавливает первый индекс
    void setFirstIndex(const QModelIndex& index);

    //! Возращает первый индекс
    QModelIndex firstIndex() const;

    //! Устанавливает второй индекс
    void setSecondIndex(const QModelIndex& index);

    //! Возращает второй индекс
    QModelIndex secondIndex() const;
    
private:
    //! Первый индекс
    QModelIndex m_firstIndex;

    //! Второй индекс
    QModelIndex m_secondIndex;

    //! Модель атрибутов
    RTPTechGroup::XmlModel::TableXMLProxyModel *m_attrModel;
};

}}

#endif // CONDITIONDELEGATE_H
