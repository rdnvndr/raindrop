#ifndef PROPNUMERATOR_H
#define PROPNUMERATOR_H

#include "ui_propnumerator.h"

#include <treexmlmodel/treexmlhashmodel.h>

#include "abstractpropeditor.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования нумераторов
/*! Диалог предназначен для редактирования нумераторов
*/

class PropNumerator : public AbstractPropEditor, private Ui::PropNumerator
{
    Q_OBJECT

public:
    //! Конструктор диалога редактирования нумераторов
    explicit PropNumerator(QWidget *parent = 0);

    //! Деструктор диалога редактирования нумераторов
    virtual ~PropNumerator();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Получение индетификатора по индексу
    QString dataId(const QModelIndex &index);

public slots:
    //! Установка текущего нумератора
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Перевод нумераторов в режим редактирования
    void edit(bool flag);
};

}}

#endif // PROPNUMERATOR_H
