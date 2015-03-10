#ifndef ABSTRACTEDITORWIDGET_H
#define ABSTRACTEDITORWIDGET_H

#include <QDataWidgetMapper>
#include <QWidget>

#include <treexmlmodel/treexmlhashmodel.h>

using namespace  RTPTechGroup::XmlModel;

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования
/*! Диалог предназначен для редактирования
 */

class AbstractEditorWidget: public QWidget
{
    Q_OBJECT
public:
    //! Конструктор редактора
    explicit AbstractEditorWidget(QWidget *parent);

    //! Деструктор редактора
    virtual ~AbstractEditorWidget();

    //! Установка модели структуры классов
    virtual void setModel(TreeXmlHashModel *model);

    //! Получение модели структуры классов
    virtual TreeXmlHashModel* model();

    //! Получение маппера данных
    virtual QDataWidgetMapper *dataMapper();

    //! Проверка на возможность удаления
    virtual bool isRemove(const QModelIndex &srcIndex) = 0;

    //! Проверка запонены ли данные
    virtual bool isEmpty() = 0;

public slots:
    //! Добавление строки
    virtual bool add(const QString &tag);

    //! Удаление строки
    virtual void remove();

    //! Удаление пустой строки
    virtual bool removeEmpty();

    //! Установка текущей строки
    virtual void setCurrent(const QModelIndex &index);

    //! Перевод в режим редактирования
    virtual void edit(bool flag = true) = 0;

    //! Применение изменений
    virtual void submit();

    //! Отмена изменений
    virtual void revert();

    //! Удаление строки
        void rowsRemoved(const QModelIndex &index,int start,int end);

    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr,
                       const QModelIndex &index, int role = Qt::DisplayRole);

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении данных
    void currentIndexChanged(const QModelIndex &index);

private:
    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Mapper для свойств справочника
    QDataWidgetMapper* m_mapper;

    //! Хранит индекс предыдущей активной справочника
    QPersistentModelIndex  m_oldIndex;
};

}}

#endif // ABSTRACTEDITORWIDGET_H
