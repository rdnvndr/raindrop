#ifndef COMPOSITIONWIDGET_H
#define COMPOSITIONWIDGET_H

#include "ui_compositionwidget.h"
#include <treexmlmodel/tablexmlproxymodel.h>
#include <treexmlmodel/treexmlmodel.h>
#include <QDataWidgetMapper>

//! Диалог редактирования составов класса
/*! Диалог предназначен для редактирования списка составов
    класса
*/

class CompositionWidget : public QWidget, private Ui::CompositionWidget
{
    Q_OBJECT
    
public:
    //! Конструктор редактора составов класса
    explicit CompositionWidget(QWidget *parent = 0);

    //! Деструктор редактора составов класса
    virtual ~CompositionWidget();

      //! Установка модели для редактирования составов класса
    void setModel(TreeXMLModel *model);

public slots:
    //! Добавление состава класса
    void add();

    //! Удаление состава класса
    void remove();

    //! Вызов редактирования состава класса
    void edit();

    //! Установка отображения родительских составов класса
    void showParent(bool flag);

    //! Установка родителя состава
    void setRootIndex(QModelIndex index);

signals:
    //! Сигнал об добавлении данных
    void dataAdded(QModelIndex index);

    //! Сигнал об вызове редактирования данных
    void dataEdited(QModelIndex index);

    //! Сигнал об изменении данных
    void dataChanged(QModelIndex index);

    //! Сигнал об удалении данных
    void dataRemoved(QModelIndex index);

    //! Сигнал об изменении текущего состава
    void currentIndexChanged(QModelIndex index);

private:
    //! Получение данных модели
    QVariant modelData(QString typeName, QString attr, const QModelIndex &index);

    //! Прокси модель для составов класса
    TableXMLProxyModel* m_compositionModel;

    //! Модель структуры классов
    TreeXMLModel* m_model;
};

#endif // COMPOSITIONWIDGET_H
