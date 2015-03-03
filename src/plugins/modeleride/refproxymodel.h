#ifndef REFPROXYMODEL_H
#define REFPROXYMODEL_H

#include <treexmlmodel/modifyproxymodel.h>
#include <treexmlmodel/treexmlmodel.h>

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Прокси модель справочника
/*! Прокси модель предназначена для редактирования справочника
 */

class RefProxyModel : public ModifyProxyModel
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit RefProxyModel(QObject *parent = 0);

    //! Получение данных модели
    QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const;
};

}}

#endif // REFPROXYMODEL_H
