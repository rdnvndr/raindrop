#ifndef OBJECTCOLLECTION_H
#define OBJECTCOLLECTION_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QPointer>
//! Класс колекции объектов
/*!
  Управляет колекцией объектов. Предоставляет стандарный интерфейс доступа к объетам.
  */
class ObjectCollection : public QObject
{
    Q_OBJECT
public:
    explicit ObjectCollection(QObject *parent = 0);

    //Операции с объектами
    void addObject(QPointer<QObject> obj);                                   //! Довбление объета в коллекцю
    void removeObject(QPointer<QObject> obj);                                //! Удаление объекта из коллекции
    QPointer<QObject> getObjectByName(const QString &name) const;            //! Получение объекта по имени
    QPointer<QObject> getObjectByClassName(const QString &className) const;  //! Получение объекта по имени его класса

signals:

public slots:

private:
    QHash<QString, QObject*> m_objects;

};

#endif // OBJECTCOLLECTION_H
