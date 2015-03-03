#ifndef PROPREFGROUP_H
#define PROPREFGROUP_H

#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <QDataWidgetMapper>

#include "ui_proprefgroup.h"

namespace RTPTechGroup {
namespace ModelerIde {

using namespace RTPTechGroup::XmlModel;

//! Диалог редактирования группы справочника
/*! Диалог предназначен для редактирования группы справочника
*/

class PropRefGroup : public QWidget, private Ui::PropRefGroup
{
    Q_OBJECT

public:
    //! Конструктор диалога редактирования группы справочника
    explicit PropRefGroup(QWidget *parent = 0);

    //! Деструктор диалога редактирования группы справочника
    virtual ~PropRefGroup();


    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Получение модели структуры классов
    TreeXmlHashModel* model();

    //! Проверка на возможность удаления группы справочника
    static bool isRemove(const QModelIndex &srcIndex);

    //! Проверка запонена ли имя сущности группы справочника
    bool isEmpty();

public slots:
    //! Добавление сущности группы справочника
    void add();

    //! Удаление сущности группы справочника
    void remove();

    //! Удаление пустой сущности группы справочника
    bool removeEmpty();

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении текущей группы справочника
    void currentIndexChanged(const QModelIndex &index);

    //! Сигнал о редактировании группы справочника
    void edited(bool flag);

public slots:
    //! Установка текущей группы справочника
    void setCurrentRefGroup(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Закрытие вкладки/подокна
    void closeTab(const QModelIndex &index);

    //! Перевод группы справочника в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений группы справочника
    void submit();

    //! Отмена изменений группы справочника
    void revert();

    //! Удаление группы справочника
    void rowsRemoved(const QModelIndex &index,int start,int end);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Mapper для свойств группы справочника
    QDataWidgetMapper* m_mapper;

    //! Хранит индекс предыдущей активной группы сущности ЕИ
    int  m_oldIndex;
};

}}

#endif // PROPREFGROUP_H
