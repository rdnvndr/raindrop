#ifndef CONDITIONPROXYMODEL_H
#define CONDITIONPROXYMODEL_H

#include <QAbstractProxyModel>

#include <treexmlmodel/modifyproxymodel.h>

namespace RTPTechGroup {
namespace ModelerIde {

//! Прокси модель условий фильтра
/*! Прокси модель предназначена для редактирования условий фильтра
 */

class ConditionProxyModel : public RTPTechGroup::XmlModel::ModifyProxyModel
{
    Q_OBJECT
public:
    //! Конструктор
    explicit ConditionProxyModel(QObject *parent = 0);

    //! Получение количества колонок
    qint32 columnCount (const QModelIndex& parent = QModelIndex()) const ;

    //! Получение данных модели
    QVariant data(const QModelIndex &proxyIndex, qint32 role = Qt::DisplayRole) const;

    //! Запись данных в модель
    bool setData(const QModelIndex &proxyIndex, const QVariant &value,
                 qint32 role = Qt::EditRole);

    //! Вставка строки
    bool insertRows(qint32 row, qint32 count, const QModelIndex &parent);

    //! Вставка строки
    QModelIndex insertLastRows(qint32 row, qint32 count, const QModelIndex &parent);
};

}}

#endif // CONDITIONPROXYMODEL_H
