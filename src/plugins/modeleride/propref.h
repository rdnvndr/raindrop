#ifndef PROPREF_H
#define PROPREF_H

#include "ui_propref.h"

#include <QDataWidgetMapper>
#include <QStringListModel>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>

#include "abstractpropeditor.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования справочника
/*! Диалог предназначен для редактирования справочника
*/

class PropRef : public AbstractPropEditor, private Ui::PropRef
{
    Q_OBJECT

public:
     //! Конструктор диалога редактирования справочника
    explicit PropRef(QWidget *parent = 0);

    //! Деструктор диалога редактирования справочника
    virtual ~PropRef();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

public slots:
    //! Установка текущего справочника
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Перевод справочника в режим редактирования
    void edit(bool flag);
};

}}

#endif // PROPREF_H
