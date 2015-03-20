#ifndef PERMDELEGATE_H
#define PERMDELEGATE_H

#include <QStyledItemDelegate>

#include "xmldelegate.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Делегат XML модели для прав доступа
/*! Данный делегат предназначен для ввода данных в XML модель
    TreeXMLModel для прав доступа
 */

class PermDelegate : public XmlDelegate
{
    Q_OBJECT
public:
    //! Конструктор делегата
    explicit PermDelegate(QObject *parent = 0);

    //! Деструктор делегата
    virtual ~PermDelegate();

    //! Предоставление редактора
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    //! Перерисовка делегата
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

private:
    const TreeXmlHashModel *hashModel(const QAbstractItemModel *model) const;
};

}}

#endif // PERMDELEGATE_H
