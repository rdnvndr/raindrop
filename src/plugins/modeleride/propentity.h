#ifndef PROPENTITY_H
#define PROPENTITY_H

#include <QDataWidgetMapper>
#include <QStringListModel>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>

#include "ui_propentity.h"
#include "abstractpropeditor.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования сущности единицы измерения
/*! Диалог предназначен для редактирования сущности единицы измерения
*/

class PropEntity : public AbstractPropEditor, private Ui::PropEntity
{
    Q_OBJECT

public:
    //! Конструктор диалога редактирования сущности единицы измерения
    explicit PropEntity(QWidget *parent = 0);

    //! Деструктор диалога редактирования сущности единицы измерения
    virtual ~PropEntity();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

public slots:
    //! Установка текущей сущности единицы измерения
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Перевод сущности ЕИ в режим редактирования
    void edit(bool flag);
};

}}

#endif // PROPENTITY_H
