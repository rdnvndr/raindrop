#ifndef PROPENTITY_H
#define PROPENTITY_H

#include "ui_propentity.h"
#include <treexmlmodel/treexmlhashmodel.h>
#include <treexmlmodel/tablexmlproxymodel.h>
#include <QDataWidgetMapper>
#include <QStringListModel>

//! Диалог редактирования сущности единицы измерения
/*! Диалог предназначен для редактирования сущности единицы измерения
*/

class PropEntity : public QWidget, private Ui::PropEntity
{
    Q_OBJECT

public:
    //! Конструктор диалога редактирования сущности единицы измерения
    explicit PropEntity(QWidget *parent = 0);

    //! Деструктор диалога редактирования сущности единицы измерения
    virtual ~PropEntity();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Получение модели структуры классов
    TreeXmlHashModel* model();

public slots:
    //! Установка текущей сущности единицы измерения
    void setCurrentEntity(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Закрытие вкладки/подокна
    void closeTab(const QModelIndex &index);

    //! Перевод сущности ЕИ в режим редактирования
    void edit(bool flag);

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;
};

#endif // PROPENTITY_H
