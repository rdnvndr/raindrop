#ifndef ATTRWIDGET_H
#define ATTRWIDGET_H

#include "ui_attrwidget.h"

#include <QStringListModel>
#include <QDataWidgetMapper>

#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

#include "attrgroupproxymodel.h"
#include "abstractitemwidget.h"

using namespace RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования атрибутов класса
/*! Диалог предназначен для редактирования атрибутов класса
    и состава
 */

class AttrWidget : public AbstractItemWidget, private Ui::AttrWidget
{
    Q_OBJECT
    
public:
    //! Конструктор класса
    explicit AttrWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~AttrWidget();

    //! Установка модели для редактирования атрибутов класса
    void setModel(TreeXmlHashModel *model);

public slots:
    //! Добавление атрибута класса
    void add();

    //! Установка текущего атрибута класса
    void setCurrent(const QModelIndex &index);

    //! Применение изменений атрибута класса
    void submit();

    //! Проверка находится ли в режиме редактирования
    bool isEdit();

    //! Перевод в режим редактирования атрибута класса
    void edit(bool flag = true);

    //! Установка отображения родительских атрибутов класса
    void showParentAttr(bool flag);

    //! Обработка изменения типа атрибута
    void changeType(const QString &typeName);

    //! Установка родителя атрибутов
    void setRootIndex(const QModelIndex &index);

    //! Сообщение о неверном имени атрибута
    void validateAttrName(QValidator::State state) const;

    //! Очистка поля ввода ЕИ
    void cleanUnit();

    //! Очистка поля ввода списка значений
    void cleanLov();

private:
    //! Прокси модель для групп атрибутов класса
    AttrGroupProxyModel *m_attrGroupModel;

    //! Список типов атрибута
    QStringListModel *m_typeAttrModel;
};

}}

#endif // ATTRWIDGET_H
