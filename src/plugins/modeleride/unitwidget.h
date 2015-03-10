#ifndef UNITWIDGET_H
#define UNITWIDGET_H

#include <QStringListModel>
#include <QDataWidgetMapper>

#include "ui_unitwidget.h"
#include <treexmlmodel/modifyproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования ЕИ
/*! Диалог предназначен для редактирования ЕИ
*/
class UnitWidget : public QWidget, private Ui::UnitWidget
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit UnitWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~UnitWidget();

    //! Установка модели для редактирования ЕИ
    void setModel(TreeXmlHashModel *model);

    //! Возращает прокси модель ЕИ
    QAbstractProxyModel *proxyModel();

    //! Проверка на возможность удаления ЕИ
    static bool isRemove(const QModelIndex &srcIndex);

signals:
    //! Сигнал об изменении корневого индекса в прокси ЕИ
    void proxyIndexChanged(const QModelIndex &index);

public slots:
    //! Добавление ЕИ
    void add();

    //! Удаление ЕИ
    void remove();

    //! Применение изменений ЕИ
    void submit();

    //! Перевод в режим редактирования
    void edit(bool flag = true);

    //! Отмена изменений ЕИ
    void revert();

    //! Установка родителя ЕИ
    void setRootIndex(const QModelIndex &index);

private:

    //! Прокси модель для ЕИ
    ModifyProxyModel* m_unitModel;

    //! Модель структуры классов
    TreeXmlHashModel* m_model;
};

}}

#endif // UNITWIDGET_H
