#ifndef COMPOSITIONWIDGET_H
#define COMPOSITIONWIDGET_H

#include "ui_compositionwidget.h"
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>
#include <QDataWidgetMapper>

//! Диалог редактирования составов класса
/*! Диалог предназначен для редактирования списка составов
    класса
*/

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

class CompositionWidget : public QWidget, private Ui::CompositionWidget
{
    Q_OBJECT
    
public:
    //! Конструктор редактора составов класса
    explicit CompositionWidget(QWidget *parent = 0);

    //! Деструктор редактора составов класса
    virtual ~CompositionWidget();

      //! Установка модели для редактирования составов класса
    void setModel(TreeXmlHashModel *model);

public slots:
    //! Добавление состава класса
    void add();

    //! Удаление состава класса
    void remove();

    //! Установка текущего состава класса
    void setCurrent(const QModelIndex &index);

    //! Применение изменений состава класса
    void submit();

    //! Проверка находится ли в режиме редактирования
    bool isEdit();

    //! Перевод в режим редактирования состава класса
    void edit(bool flag = true);

    //! Перемещение состава выше
    void up();

    //! Перемещение состава ниже
    void down();

    //! Отмена изменений состава класса
    void revert();

    //! Установка отображения родительских составов класса
    void showParent(bool flag);

    //! Установка родителя состава
    void setRootIndex(QModelIndex index);

signals:  

    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении текущего состава
    void currentIndexChanged(const QModelIndex &index);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Прокси модель для составов класса
    TableXMLProxyModel* m_compositionModel;

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Mapper для свойств атрибутов
    QDataWidgetMapper* m_mapperComp;
};

}}

#endif // COMPOSITIONWIDGET_H
