#ifndef MIMEDATAINDEX_H
#define MIMEDATAINDEX_H

#include <QMimeData>
#include <QPersistentModelIndex>
#include <QModelIndex>
#include <QList>
#include <QStringList>

//! MimeDataXml контейнер для записи информации с mime типом
/*! MimeDataXml расширенный контейнер QMimeData. В нем можно
 *  передавать экземпляры классов типа QModelIndex
 */

typedef QList<QPersistentModelIndex> PersistentIndexes;

class MimeDataIndex : public QMimeData
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit MimeDataIndex();

    //! Записывает объект
    virtual void setIndexes(PersistentIndexes indexes);

    //! Возращает объект
    PersistentIndexes indexes() const;

    //! Возращает true если объект внутри существует
    virtual bool hasIndexes() const;

    //! Возращает список mime типов записанной информации
    QStringList formats() const;

private:
    //! Предназначено для хранения индекса
    PersistentIndexes m_indexes;
    
};

#endif
