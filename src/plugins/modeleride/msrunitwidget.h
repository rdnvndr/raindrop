#ifndef MSRUNITWIDGET_H
#define MSRUNITWIDGET_H

#include <QStringListModel>
#include <QDataWidgetMapper>

#include "ui_msrunitwidget.h"
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>

class MsrUnitWidget : public QWidget, private Ui::MsrUnitWidget
{
    Q_OBJECT

public:
    //! Конструктор класса
    explicit MsrUnitWidget(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~MsrUnitWidget();

    //! Установка модели для редактирования ЕИ
    void setModel(TreeXmlHashModel *model);

public slots:
    //! Добавление ЕИ
    void add();

    //! Удаление ЕИ
    void remove();

    //! Установка текущего ЕИ
    void setCurrent(const QModelIndex &index);

    //! Применение изменений ЕИ
    void submit();

    //! Перевод в режим редактирования
    void edit(bool flag = true);

    //! Отмена изменений ЕИ
    void revert();

    //! Установка родителя ЕИ
    void setRootIndex(const QModelIndex &index);

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении текущего ЕИ
    void currentIndexChanged(const QModelIndex &index);

    //! Сигнал о редактировании ЕИ
    void edited(bool flag);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr,
                       const QModelIndex &index);

    //! Прокси модель для ЕИ
    TableXMLProxyModel* m_unitModel;

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Mapper для свойств ЕИ
    QDataWidgetMapper* m_mapperUnit;
};

#endif // MSRUNITWIDGET_H
