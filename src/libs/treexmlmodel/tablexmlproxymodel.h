#ifndef TABLEXMLPROXYMODEL_H
#define TABLEXMLPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QStringList>
#include "treexmlmodelglobal.h"

//! Класс прокси модель  для XML данных класса TreeXMLModel
/*! Данный класс предназначен для табличного отображения атрибутов
    класса TreeXMLModel.Позволяет отображать наследуемые тэги, сортировать
    и фильтровать данные. \n
    Пример использования:
    \code
    TableXMLProxyModel *m_attrModel = new TableXMLProxyModel();
    QStringList tags;
    tags << "Attribute";
    m_attrModel->setAttributeTags(tags);
    \endcode
*/
class XMLMODELLIB TableXMLProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    //! Конструктор модели
    TableXMLProxyModel();

    //! Метод который определяет отображать ли указанную строку
    virtual bool filterAcceptsRow (int row, const QModelIndex &parent) const;

    //! Устанавливает индекс модели, в которой будут отображаться только атрибуты
    void setFilterIndex(const QModelIndex &parent);

    //! Получение индекса модели, в которой будут отображаться только атрибуты
    QModelIndex filterIndex();

    //! Получение тэгов, которые являются атрибутами
    QStringList attributeTags();

     //! Установка тэгов, которые являются атрибутами
    void setAttributeTags(QStringList list);

    //! Установка количества столбцов
    void setColumnCount(int column);

    //! Количество столбцов
    virtual int columnCount(const QModelIndex &parent) const;

private:
    //! Индекс модели, в котором отображаются только атрибуты
    QModelIndex m_index;
    //! Список тэгов, которые являются атрибутами
    QStringList m_tags;
    //! Количество столбцов
    int m_columnCount;
};

#endif // TABLEXMLPROXYMODEL_H
