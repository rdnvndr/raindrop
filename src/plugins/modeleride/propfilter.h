#ifndef PROPFILTER_H
#define PROPFILTER_H

#include "ui_propfilter.h"

namespace RTPTechGroup {
namespace ModelerIde {

//! Диалог редактирования фильтра
/*! Диалог предназначен для редактирования фильтра
*/

class PropFilter : public QWidget, private Ui::PropFilter
{
    Q_OBJECT
    
public:
    //! Конструктор класса
    explicit PropFilter(QWidget *parent = 0);

    //! Деструктор класса
    virtual ~PropFilter();

    //! Установка модели структуры классов
    void setModel(TreeXmlHashModel *model);

    //! Получение модели структуры классов
    TreeXmlHashModel* model();

public slots:
    //! Установка текущего класса
    void setCurrent(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Закрытие вкладки/подокна
    void closeTab(const QModelIndex &index = QModelIndex());

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXmlHashModel* m_model;
};

}}

#endif // PROPFILTER_H
