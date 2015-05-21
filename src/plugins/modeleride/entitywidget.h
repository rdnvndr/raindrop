#ifndef ENTITYWIDGET_H
#define ENTITYWIDGET_H

#include "ui_entitywidget.h"

#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QStringListModel>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

#include "abstracteditorwidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования сущности ЕИ
/*! Диалог предназначен для редактирования сущности ЕИ
 */
class EntityWidget : public AbstractEditorWidget, private Ui::EntityWidget
{
    Q_OBJECT

public:
    //! Конструктор редактора сущности ЕИ
    explicit EntityWidget(QWidget *parent = 0);

    //! Деструктор редактора сущности ЕИ
    virtual ~EntityWidget();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Проверка запонено ли имя сущности ЕИ
    bool isEmpty();

public slots:
    //! Добавление сущности ЕИ
    void add();

    //! Перевод сущности ЕИ в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений сущности ЕИ
    void submit();

    //! Выполняется до удаления сущности ЕИ
    void rowsAboutToBeRemoved(const QModelIndex &parent,int start,int end);

    //! Установка модели ЕИ
    void setUnitModel(QAbstractItemModel *model);

    //! Установка корневого индекса модели ЕИ
    void setUnitRootIndex(const QModelIndex &index);

    //! Установка колонки отображения для ЕИ
    void setUnitColumn(int column);

private slots:
    //! Обработка изменения базовой ЕИ
    void changeUnit(int current);

signals:
    //! Сигнал о редактировании сущности ЕИ
    void edited(bool flag);

};

}}

#endif // ENTITYWIDGET_H
