#ifndef UNITDELEGATE_H
#define UNITDELEGATE_H

#include <QStyledItemDelegate>

namespace RTPTechGroup {
namespace ModelerIde {

//! Делегат для модели ЕИ
/*! Данный делегат предназначен для ввода данных ЕИ в XML модель
 */
class UnitDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit UnitDelegate(QObject *parent = 0);

    //! Запись данных в модель
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    //! Предоставление редактора
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //! Делегат должен предоставить функцию копирования данных модели в редактор.
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

signals:

public slots:

};

}}

#endif // UNITDELEGATE_H
