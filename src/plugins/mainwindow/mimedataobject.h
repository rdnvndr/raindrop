#ifndef MIMEDATAOBJECT_H
#define MIMEDATAOBJECT_H

#include <QMimeData>
#include <QAction>

class MimeDataObject : public QMimeData
{
    Q_OBJECT
public:
    explicit MimeDataObject();
    void setObject(QAction *object);
    QObject *object() const;
    bool hasObject() const;
private:
    QObject *m_object;
    
};

#endif // MIMEDATAOBJECT_H
