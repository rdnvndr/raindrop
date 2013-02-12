#ifndef MIMEDATAOBJECT_H
#define MIMEDATAOBJECT_H

#include <QMimeData>
#include <QAction>

class MimeDataObject : public QMimeData
{
    Q_OBJECT
public:
    explicit MimeDataObject();

    virtual void setObject(QAction *object);
    virtual QObject *object() const;
    virtual bool hasObject() const;

    QStringList formats() const;
private:
    QObject *m_object;
    
};

#endif // MIMEDATAOBJECT_H
