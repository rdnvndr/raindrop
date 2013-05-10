#ifndef PROPFILTER_H
#define PROPFILTER_H

#include "ui_propfilter.h"

class PropFilter : public QWidget, private Ui::PropFilter
{
    Q_OBJECT
    
public:
    explicit PropFilter(QWidget *parent = 0);

    //! Установка модели структуры классов
    void setModel(TreeXMLModel *model);

    //! Получение модели структуры классов
    TreeXMLModel* model();

public slots:
    //! Установка текущего класса
    void setCurrentClass(const QModelIndex &index);

    //! Установка наименования вкладки/подокна
    void setTabName(const QModelIndex &index);

    //! Закрытие вкладки/подокна
    void closeTab(const QModelIndex &index = QModelIndex());

private:
    //! Получение данных модели
    QVariant modelData(const QString &tag, const QString &attr, const QModelIndex &index);

    //! Модель структуры классов
    TreeXMLModel* m_model;
};

#endif // PROPFILTER_H
