#ifndef TAGXMLITEM_H
#define TAGXMLITEM_H

#include <QObject>
#include <QDomNode>
#include <QMap>
#include <QStringList>

#include "treexmlmodelglobal.h"

//! Класс узла дерева TreeXMLModel
/*! Класс предназначен для построения дерева xml модели
 */

class XMLMODELLIB TagXmlItem: public QObject
{
    Q_OBJECT
public:
    //! Конструктор узла дерева
    TagXmlItem(QDomNode &node, TagXmlItem *parent = 0);

    //! Деструктор узла дерева
    virtual ~TagXmlItem();

    //! Возращает родителя
    TagXmlItem *parent();

    //! Возращает тэг
    QDomNode node() const;

    //! Возращает имя узла
    QString nodeName() const;

    //! Устанавливает имя узла
    void setNodeName(const QString& tagName);

    //! Возращает значение атрибута тэга
    QVariant value(const QString& attr);

    //! Устанавливает значение атрибута тэга
    void setValue(const QString& attr, const QVariant& val);

    //! Возращает количество записей
    int count(QStringList tags = QStringList(),
              QStringList parentTags = QStringList());

    //! Возращает True при наличии записей
    int hasChildren(QStringList tags = QStringList(),
                    QStringList parentTags = QStringList());

    //! Возращает потомка
    TagXmlItem *child(int i, QStringList tags = QStringList(),
                             QStringList parentTags = QStringList());

    //! Возращает номер потомка
    int childNumber(TagXmlItem *child, QStringList tags, QStringList parentTags);

    //! Вставка узла
    bool insertChild(const QString &tagName);

    //! Вставка узла
    bool insertChild(const QString &tagName, int i,
                     QStringList tags = QStringList(),
                     QStringList parentTags = QStringList());

    //! Удаление узла
    bool removeChild(const int &row);

    //! Проверка на возможность удаление узла
    bool checkRemoveChild(const int &row);

    //! Возращает True если унаследован
    bool isInherited();

signals:
    //! Сигнал об уничтожении объекта
    void destroyedItem(TagXmlItem* item);

private slots:
    //! Удаление наследуемого узла из списка
    void removeInheritedItem(TagXmlItem* parent);

private:
    //! Тэг
    QDomNode m_domNode;

    //! Родитель
    TagXmlItem *m_parentItem;

    //! Список потомков
    QList<TagXmlItem*> m_childItems;

    //! Список наследников
    QMap<TagXmlItem*, TagXmlItem*> m_inherited;
};

#endif
