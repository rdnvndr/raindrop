#ifndef MSRENTITYWIDGET_H
#define MSRENTITYWIDGET_H

#include "ui_msrentitywidget.h"

#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QStringListModel>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

#include "abstracteditorwidget.h"

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования сущности ЕИ
/*! Диалог предназначен для редактирования сущности ЕИ
 */
class MsrEntityWidget : public AbstractEditorWidget, private Ui::MsrEntityWidget
{
    Q_OBJECT

public:
    //! Конструктор редактора сущности ЕИ
    explicit MsrEntityWidget(QWidget *parent = 0);

    //! Деструктор редактора сущности ЕИ
    virtual ~MsrEntityWidget();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Проверка на возможность удаления сущности ЕИ
    bool isRemove(const QModelIndex &srcIndex);

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

#endif // MSRENTITYWIDGET_H
