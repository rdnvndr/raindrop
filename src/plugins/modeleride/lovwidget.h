#ifndef LOVWIDGET_H
#define LOVWIDGET_H

#include "ui_lovwidget.h"
#include "treexmlmodel/treexmlhashmodel.h"
#include <QDataWidgetMapper>

class LovWidget : public QWidget, private Ui::LovWidget
{
    Q_OBJECT

public:
    //! Конструктор редактора списка значений
    explicit LovWidget(QWidget *parent = 0);

     //! Деструктор редактора списка значений
    virtual ~LovWidget();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Проверка на возможность удаления списка значений
    static bool isRemove(const QModelIndex &srcIndex);

    //! Проверка запонено ли имя списка значений
    bool isEmpty();

public slots:
    //! Добавление списка значений
    void add();

    //! Удаление списка значений
    void remove();

    //! Удаление пустого списка значений
    void removeEmpty();

    //! Установка текущего списка значений
    void setCurrent(const QModelIndex &index);

    //! Перевод списка значений в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений списка значений
    void submit();

    //! Отмена изменений списка значений
    void revert();

    //! Удаление списка значений
    void rowsRemoved(const QModelIndex &index,int start,int end);

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении текущего списка значений
    void currentIndexChanged(const QModelIndex &index);

    //! Сигнал о редактировании списка значений
    void edited(bool flag);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr,
                       const QModelIndex &index, int role = Qt::DisplayRole);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Mapper для свойств списка значений
    QDataWidgetMapper* m_mapper;

    //! Хранит индекс предыдущего активного списка значений
    int  m_oldIndex;

};

#endif // LOVWIDGET_H
