#ifndef QUANTITYWIDGET_H
#define QUANTITYWIDGET_H

#include "ui_quantitywidget.h"

#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QStringListModel>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

#include "abstracteditorwidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования величины
/*! Диалог предназначен для редактирования величины
 */
class QuantityWidget : public AbstractEditorWidget, private Ui::QuantityWidget
{
    Q_OBJECT

public:
    //! Конструктор редактора величины
    explicit QuantityWidget(QWidget *parent = 0);

    //! Деструктор редактора величины
    virtual ~QuantityWidget();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Проверка запонено ли имя величины
    bool isEmpty();

public slots:
    //! Добавление величины
    void add();

    //! Перевод величины в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений величины
    void submit();

    //! Выполняется до удаления величины
    void rowsAboutToBeRemoved(const QModelIndex &parent,qint32 start,qint32 end);

    //! Установка модели ЕИ
    void setUnitModel(QAbstractItemModel *model);

    //! Установка корневого индекса модели ЕИ
    void setUnitRootIndex(const QModelIndex &index);

    //! Установка колонки отображения для ЕИ
    void setUnitColumn(qint32 column);

private slots:
    //! Обработка изменения базовой ЕИ
    void changeUnit(qint32 current);

signals:
    //! Сигнал о редактировании величины
    void edited(bool flag);

};

}}

#endif // QUANTITYWIDGET_H
