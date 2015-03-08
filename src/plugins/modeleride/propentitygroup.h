#ifndef PROPENTITYGROUP_H
#define PROPENTITYGROUP_H

#include <QDataWidgetMapper>

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>

#include "ui_propentitygroup.h"
#include "abstractpropeditor.h"

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования группы сущности ЕИ
/*! Диалог предназначен для редактирования группы сущности ЕИ
*/
class PropEntityGroup : public AbstractPropEditor, private Ui::PropEntityGroup
{
    Q_OBJECT

public:
    //! Конструктор диалога редактирования группы сущности ЕИ
    explicit PropEntityGroup(QWidget *parent = 0);

    //! Деструктор диалога редактирования группы сущности ЕИ
    virtual ~PropEntityGroup();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Проверка на возможность удаления группы сущности ЕИ
    static bool isRemove(const QModelIndex &srcIndex);

    //! Проверка запонена ли имя сущности группы сущности ЕИ
    bool isEmpty();

public slots:
    //! Добавление сущности группы сущности ЕИ
    void add();

    //! Удаление сущности группы сущности ЕИ
    void remove();

    //! Удаление пустой сущности группы сущности ЕИ
    bool removeEmpty();

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении текущей группы сущности ЕИ
    void currentIndexChanged(const QModelIndex &index);

    //! Сигнал о редактировании группы сущности ЕИ
    void edited(bool flag);

public slots:
    //! Установка текущей группы сущности ЕИ
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Перевод группы сущности ЕИ в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений группы сущности ЕИ
    void submit();

    //! Отмена изменений группы сущности ЕИ
    void revert();

    //! Удаление группы сущности ЕИ
    void rowsRemoved(const QModelIndex &index,int start,int end);

private:

    //! Mapper для свойств группы сущности ЕИ
    QDataWidgetMapper* m_mapper;

    //! Хранит индекс предыдущей активной группы сущности ЕИ
    QPersistentModelIndex  m_oldIndex;
};

}}

#endif // PROPENTITYGROUP_H
