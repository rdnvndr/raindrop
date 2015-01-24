#ifndef ATTRGROUPPROXYMODEL_H
#define ATTRGROUPPROXYMODEL_H

#include <treexmlmodel/tablexmlproxymodel.h>

class AttrGroupProxyModel : public TableXMLProxyModel
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit AttrGroupProxyModel(QObject *parent = 0);

    //! Деструктор класаа
    virtual ~AttrGroupProxyModel();

    //! Указание столбца с уникальными значениями
    void setUniqueColumn(int column);

    //! Возращает столбец с уникальными значениями
    int uniqueColumn() const;

protected:
    //! Метод который определяет отображать ли указанную строку
    virtual bool filterAcceptsRow (int row, const QModelIndex &parent) const;

private:
    int m_uniqueColumn;
};

#endif // ATTRGROUPPROXYMODEL_H
