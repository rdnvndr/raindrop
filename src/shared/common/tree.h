#ifndef TREE_H
#define TREE_H

#include <QCoreApplication>
#include <QVariant>

/*!
 * \brief Класс реализует алгоритмы древовидной динамической структуры данных.
 *
 * Класс служит как базовый для реализации древовидной структуры данных. Удобен для создание поверх него модели.
 */
class Tree
{
    Q_DECLARE_TR_FUNCTIONS ( Tree )
public:
    Tree();
    Tree(const Tree &other);
    virtual ~Tree();

    virtual Tree* parent() const;
    virtual Tree* firstChild() const;
    virtual Tree* lastChild() const;
    virtual Tree* nextSibling() const;
    virtual Tree* previousSibling() const;

    int childCount() const;
    virtual Tree* childAt(int index) const;
    int indexChildOf(Tree* child) const;

    virtual void appendChild(Tree* node);
    virtual void appendSibling(Tree* node);
    virtual void removeChildren();

protected:
    virtual void copy(const Tree *other);

private:
    void setParent(Tree* parent);
    void setFirstChild(Tree* firstChild);
    void setLastChild(Tree* lastChild);
    void setNextSibling(Tree* nextSibling);
    void setPreviousSibling(Tree* previousSibling);
    void remove();


    int m_childCount;

    Tree* m_nextSibling;
    Tree* m_previousSibling;
    Tree* m_parent;
    Tree* m_firstChild;
    Tree* m_lastChild;
};

Q_DECLARE_METATYPE(Tree);

#endif // TREE_H
