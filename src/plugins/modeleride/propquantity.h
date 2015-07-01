#ifndef PROPQUANTITY_H
#define PROPQUANTITY_H

#include "ui_propquantity.h"

#include <QDataWidgetMapper>
#include <QStringListModel>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>

#include "abstractpropeditor.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования величины единицы измерения
/*! Диалог предназначен для редактирования величины единицы измерения
*/

class PropQuantity : public AbstractPropEditor, private Ui::PropQuantity
{
    Q_OBJECT

public:
    //! Конструктор диалога редактирования величины единицы измерения
    explicit PropQuantity(QWidget *parent = 0);

    //! Деструктор диалога редактирования величины единицы измерения
    virtual ~PropQuantity();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Получение индетификатора по индексу
    QString dataId(const QModelIndex &index);

public slots:
    //! Установка текущей величины единицы измерения
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Перевод величины ЕИ в режим редактирования
    void edit(bool flag);
};

}}

#endif // PROPQUANTITY_H
