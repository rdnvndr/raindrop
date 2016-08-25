#ifndef REFPROXYMODEL_H
#define REFPROXYMODEL_H

#include <treexmlmodel/modifyproxymodel.h>
#include <treexmlmodel/treexmlmodel.h>

namespace RTPTechGroup {
namespace ModelerIde {

//! Прокси модель справочника
/*! Прокси модель предназначена для редактирования справочника
 */

class RefProxyModel : public RTPTechGroup::XmlModel::ModifyProxyModel
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit RefProxyModel(QObject *parent = 0);

    //! Получение данных модели
    QVariant data(const QModelIndex &proxyIndex, qint32 role = Qt::DisplayRole) const;
};

}}

#endif // REFPROXYMODEL_H
