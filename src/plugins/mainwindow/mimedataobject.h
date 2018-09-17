#ifndef MIMEDATAOBJECT_H
#define MIMEDATAOBJECT_H

#include <QMimeData>
#include <QAction>

namespace RTPTechGroup {
namespace MainWindow {

//! MimeDataObject контейнер для записи информации с mime типом
/*! MimeDataObject расширенный контейнер QMimeData. В нем можно
 *  передавай экземпляры классов типа QObject
 */

class MimeDataObject : public QMimeData
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit MimeDataObject();

    //! Записывает объект
    virtual void setObject(QAction *object);

    //! Возвращает объект
    virtual QObject *object() const;

    //! Возвращает true если объект внутри существует
    virtual bool hasObject() const;

    //! Возвращает список mime типов записанной информации
    QStringList formats() const;

private:
    //! Предназначено для хранения объекта
    QObject *m_object;
    
};

}
}

#endif // MIMEDATAOBJECT_H
