#ifndef LOVWIDGET_H
#define LOVWIDGET_H

#include "ui_lovwidget.h"

#include <QDataWidgetMapper>
#include <QStringListModel>

#include <treexmlmodel/treexmlhashmodel.h>

#include "abstracteditorwidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования списка значений
/*! Диалог предназначен для редактирования списка значений
*/

class LovWidget : public AbstractEditorWidget, private Ui::LovWidget
{
    Q_OBJECT

public:
    //! Конструктор редактора списка значений
    explicit LovWidget(QWidget *parent = 0);

    //! Деструктор редактора списка значений
    virtual ~LovWidget();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

    //! Проверка заполнено ли имя списка значений
    bool isEmpty();

public slots:
    //! Добавление списка значений
    void add();

    //! Установка текущего списка значений
    void setCurrent(const QModelIndex &index);

    //! Перевод списка значений в режим редактирования
    void edit(bool flag);

    //! Применение изменений списка значений
    void submit();

    //! Сообщение о неверном имени списка значений
    void validateLovName(QValidator::State state) const;

signals:
    //! Сигнал о редактировании списка значений
    void edited(bool flag);

private:
    //! Список типов значений
    QStringListModel *m_typeAttrModel;
};

}}

#endif // LOVWIDGET_H
