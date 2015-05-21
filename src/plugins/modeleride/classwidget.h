#ifndef CLASSWIDGET_H
#define CLASSWIDGET_H

#include "ui_classwidget.h"

#include <QDataWidgetMapper>
#include <QStringListModel>
#include <QMessageBox>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

#include "abstracteditorwidget.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования класса
/*! Диалог предназначен для редактирования класса
 */

class ClassWidget : public AbstractEditorWidget, private Ui::ClassWidget
{
    Q_OBJECT
    
public:
    //! Конструктор редактора класса
    explicit ClassWidget(QWidget *parent = 0);

    //! Деструктор редактора класса
    virtual ~ClassWidget();

    //! Установка модели структуры классов
    void setModel(RTPTechGroup::XmlModel::TreeXmlHashModel *model);

public slots:
    //! Добавление класса
    void add();

    //! Установка текущего класса
    void setCurrent(const QModelIndex &index);

    //! Возращает находится ли в режиме редактирования
    bool isEdit();

    //! Перевод класса в режим редактирования
    void edit(bool flag = true);

    //! Проверка запонены ли данные
    bool isEmpty();

    //! Применение изменений класса
    void submit();

    //! Сообщение о неверном имени класса
    void validateClassName(QValidator::State state) const;

private:

    //! Список типов класса
    QStringListModel *m_typeClassModel;
};

}}

#endif // CLASSWIDGET_H
