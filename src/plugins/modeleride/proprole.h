#ifndef PROPROLE_H
#define PROPROLE_H

#include "ui_proprole.h"
#include "abstractpropeditor.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования роли
/*! Диалог предназначен для редактирования роли
*/

class PropRole : public AbstractPropEditor, private Ui::PropRole
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit PropRole(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~PropRole();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Получение индетификатора по индексу
    QString dataId(const QModelIndex &index);

public slots:
    //! Установка текущей роли
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Перевод роли в режим редактирования
    void edit(bool flag);
};

}}

#endif // PROPROLE_H
