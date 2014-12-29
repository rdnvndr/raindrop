#ifndef REFITEMDELEGATE_H
#define REFITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <treexmlmodel/treexmlhashmodel.h>
#include "xmldelegate.h"

//! Делегат для модели элементов справочника
/*! Данный делегат предназначен для ввода данных элементов справочника
 * в XML модель
 */

class RefItemDelegate : public XmlDelegate
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit RefItemDelegate(QObject *parent = 0);

    //! Предоставление редактора
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //! Делегат должен предоставить функцию копирования данных модели в редактор.
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    //! Запись данных в модель
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
private:
    const TreeXmlHashModel *hashModel(const QAbstractItemModel *model) const;
    QModelIndex rootClass(QModelIndex index) const;

};

#endif // REFITEMDELEGATE_H
