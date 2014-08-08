#ifndef MSRENTITYWIDGET_H
#define MSRENTITYWIDGET_H

#include "ui_msrentitywidget.h"
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlhashmodel.h>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QStringListModel>

//! Диалог редактирования сущности ЕИ
/*! Диалог предназначен для редактирования сущности ЕИ
 */
class MsrEntityWidget : public QWidget, private Ui::MsrEntityWidget
{
    Q_OBJECT

public:
    //! Конструктор редактора сущности ЕИ
    explicit MsrEntityWidget(QWidget *parent = 0);

    //! Деструктор редактора сущности ЕИ
    virtual ~MsrEntityWidget();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Проверка на возможность удаления сущности ЕИ
    static bool isRemove(const QModelIndex &srcIndex);

    //! Проверка запонено ли имя сущности ЕИ
    bool isEmpty();

public slots:
    //! Добавление сущности ЕИ
    void add();

    //! Удаление сущности ЕИ
    void remove();

    //! Удаление пустой сущности ЕИ
    void removeEmpty();

    //! Установка текущей сущности ЕИ
    void setCurrent(const QModelIndex &index);

    //! Перевод сущности ЕИ в режим редактирования
    void edit(bool flag = true);

    //! Применение изменений сущности ЕИ
    void submit();

    //! Отмена изменений сущности ЕИ
    void revert();

    //! Удаление сущности ЕИ
    void rowsRemoved(const QModelIndex &index,int start,int end);


    //! Установка модели ЕИ
    void setUnitModel(QAbstractItemModel *model);

    //! Установка корневого индекса модели ЕИ
    void setUnitRootIndex(const QModelIndex &index);

    //! Установка колонки отображения для ЕИ
    void setUnitColumn(int column);

private slots:
    //! Обработка изменения базовой ЕИ
    void changeUnit(int current);

signals:
    //! Сигнал об изменении данных
    void dataChanged(const QModelIndex &index);

    //! Сигнал об удалении данных
    void dataRemoved(const QModelIndex &index);

    //! Сигнал об изменении текущей сущности ЕИ
    void currentIndexChanged(const QModelIndex &index);

    //! Сигнал о редактировании сущности ЕИ
    void edited(bool flag);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr,
                       const QModelIndex &index, int role = Qt::DisplayRole);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;

    //! Mapper для свойств сущности ЕИ
    QDataWidgetMapper* m_mapper;

    //! Хранит индекс предыдущей активной сущности ЕИ
    int  m_oldIndex;
};

#endif // MSRENTITYWIDGET_H
