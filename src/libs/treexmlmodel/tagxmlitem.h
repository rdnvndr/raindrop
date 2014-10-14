#ifndef TAGXMLITEM_H
#define TAGXMLITEM_H

#include <QDomNode>
#include <QHash>
#include "treexmlmodelglobal.h"

//! Класс узла дерева TreeXMLModel
class XMLMODELLIB TagXmlItem
{
public:
    //! Конструктор узла дерева
    TagXmlItem(QDomNode &node, TagXmlItem *parent = 0);

    //! Деструктор узла дерева
    virtual ~TagXmlItem();

    //! Возращает родителя
    TagXmlItem *parent();

    //! Возращает тэг
    QDomNode node() const;

    //! Возращает значение атрибута тэга
    QVariant value(const QString& attr);

    //! Устанавливает значение атрибута тэга
    void setValue(const QString& attr, const QVariant& val);

    //! Возращает количество записей
    int count(QStringList tags = QStringList(), QStringList parenttags = QStringList());

    //! Возращает True при наличии записей
    int hasChildren(QStringList tags = QStringList(), QStringList parenttags = QStringList());

    //! Возращает потомка
    TagXmlItem *child(int i, QStringList tags = QStringList(), QStringList parenttags = QStringList());

    //! Возращает номер потомка
    int childNumber(TagXmlItem *child, QStringList tags, QStringList parenttags);

    //! Вставка узла
    bool insertChild(const QString &tagname);

    //! Вставка узла
    bool insertChild(const QString &tagname, int i, QStringList tags = QStringList(), QStringList parenttags = QStringList());

    //! Удаление узла
    bool removeChild(const int &row);

    //! Проверка на возможность удаление узла
    bool checkRemoveChild(const int &row);

    //! Возращает имя узла
    QString nodeName() const;

    //! Возращает True если унаследован
    bool isInherited();
private:
    //! Тэг
    QDomNode domNode;

    //! Родитель
    TagXmlItem *parentItem;

    //! Список потомков
    QList<TagXmlItem*> childItems;

    //! Указатель на узел, где находится тэг
    TagXmlItem *locationItem;

    //! Список записей с унаследованными тэгами
    QList<TagXmlItem*> contentItems;
};

#endif
