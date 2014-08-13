#ifndef LOVVALUEWIDGET_H
#define LOVVALUEWIDGET_H

#include "ui_lovvaluewidget.h"
#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/modifyproxymodel.h>

class LovValueWidget : public QWidget, private Ui::LovValueWidget
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit LovValueWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~LovValueWidget();

    //! Установка модели для редактирования значений списка
    void setModel(TreeXmlHashModel *model);

    //! Возращает прокси модель значений списка
    ModifyProxyModel* proxyModel();

    //! Проверка на возможность удаления значений списка
    static bool isRemove(const QModelIndex &srcIndex);

signals:
    //! Сигнал об изменении корневого индекса в прокси значений списка
    void proxyIndexChanged(const QModelIndex &index);

public slots:
    //! Добавление значений списка
    void add();

    //! Удаление значений списка
    void remove();

    //! Применение изменений значений списка
    void submit();

    //! Перевод в режим редактирования
    void edit(bool flag = true);

    //! Отмена изменений значений списка
    void revert();

    //! Установка родителя значений списка
    void setRootIndex(const QModelIndex &index);

private:

    //! Прокси модель для значений списка
    ModifyProxyModel* m_lovValueModel;

    //! Модель структуры классов
    TreeXmlHashModel* m_model;
};

#endif // LOVVALUEWIDGET_H
