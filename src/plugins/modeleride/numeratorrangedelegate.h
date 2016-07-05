#ifndef NUMERATORRANGEDELEGATE_H
#define NUMERATORRANGEDELEGATE_H

#include <QStyledItemDelegate>

namespace RTPTechGroup {
namespace ModelerIde {

//! Делегат редактирования интервалов нумератора
/*! Данный делегат предназначен для редактирования интервалов нумератора
 */

class NumeratorRangeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit NumeratorRangeDelegate(QObject *parent = 0);

    //! Предоставление редактора
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
public slots:
    //! Сообщение о неверном регулярном выражении
    void validateRegExp(QValidator::State state) const;
};

}}

#endif // NUMERATORRANGEDELEGATE_H
