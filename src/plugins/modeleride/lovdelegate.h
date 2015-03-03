#ifndef LOVDELEGATE_H
#define LOVDELEGATE_H

#include <QStyledItemDelegate>

namespace RTPTechGroup {
namespace ModelerIde {

//! Делегат редактирования списка значений
/*! Данный делегат предназначен для редактирования списка значений
 */

class LovDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit LovDelegate(QObject *parent = 0);

signals:

public slots:

};

}}

#endif // LOVDELEGATE_H
