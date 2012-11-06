#ifndef TAGXMLITEM_H
#define TAGXMLITEM_H

#include <QDomNode>
#include <QHash>
#include "treexmlmodelglobal.h"

//! Класс узла дерева TreeXMLModel
class XMLMODELLIB TagXMLItem
{
public:
    //! Конструктор узла дерева
    TagXMLItem(QDomNode &node, TagXMLItem *parent = 0);

    //! Деструктор узла дерева
    virtual ~TagXMLItem();

    //! Возращает родителя
    TagXMLItem *parent();

    //! Возращает тэг
    QDomNode node() const;

    //! Возращает значение атрибута тэга
    QString value(const QString& attr);

    //! Устанавливает значение атрибута тэга
    void setValue(const QString& attr, const QString& val);

    //! Возращает количество записей
    int count(QStringList tags = QStringList(), QStringList parenttags = QStringList());

    //! Возращает потомка
    TagXMLItem *child(int i, QStringList tags = QStringList(), QStringList parenttags = QStringList());

    //! Возращает номер потомка
    int childNumber(TagXMLItem *child, QStringList tags, QStringList parenttags);

    //! Вставка узла
    bool insertChild(const QString &tagname);

    //! Удаление узла
    bool removeChild(const int &row);

    //! Проверка на возможность удаление узла
    bool checkRemoveChild(const int &row);

private:
    //! Тэг
    QDomNode domNode;

    //! Родитель
    TagXMLItem *parentItem;

    //! Список потомков
    QList<TagXMLItem*> childItems;

    //! Указатель на узел, где находится тэг
    TagXMLItem *locationItem;

    //! Список записей с унаследованными тэгами
    QList<TagXMLItem*> contentItems;
};

#endif
