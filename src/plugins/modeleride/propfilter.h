#ifndef PROPFILTER_H
#define PROPFILTER_H

#include "ui_propfilter.h"

#include "abstractpropeditor.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования фильтра
/*! Диалог предназначен для редактирования фильтра
*/

class PropFilter : public AbstractPropEditor, private Ui::PropFilter
{
    Q_OBJECT
    
public:
    //! Конструктор класса
    explicit PropFilter(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~PropFilter();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Получение индетификатора по индексу
    QString dataId(const QModelIndex &index);

public slots:
    //! Установка текущего класса
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Перевод в режим редактирования
    void edit(bool flag);
};

}}

#endif // PROPFILTER_H
