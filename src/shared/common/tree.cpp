#include "tree.h"
#include "common.h"
#include <QDebug>

Tree::Tree()
{
    m_nextSibling = 0;
    m_previousSibling = 0;

    m_parent = 0;
    m_firstChild = 0;
    m_lastChild = 0;

    m_childCount = 0;
}

Tree::Tree(const Tree &other)
{
    copy(&other);
}

Tree::~Tree()
{
    removeChildren();
    remove();

    m_nextSibling = 0;
    m_previousSibling = 0;

    m_parent = 0;
    m_firstChild = 0;
    m_lastChild = 0;
}
/*!
 * \brief Предоставляет родителя
 * \return
 */
Tree *Tree::parent() const
{
    return m_parent;
}

/*!
 * \brief Предоставляет первого потомка
 * \return
 */
Tree *Tree::firstChild() const
{
    return m_firstChild;
}

/*!
 * \brief Предоставляет последнего потомка
 * \return
 */
Tree *Tree::lastChild() const
{
    return m_lastChild;
}

/*!
 * \brief Предоставляет следующего брата
 * \return
 */
Tree *Tree::nextSibling() const
{
    return m_nextSibling;
}

/*!
 * \brief Предоставляет предидущего брата
 * \return
 */
Tree *Tree::previousSibling() const
{
    return m_previousSibling;
}

/*!
 * \brief Предоставляет количество потомков
 * \return
 */
int Tree::childCount() const
{
    return m_childCount;
}

/*!
 * \brief Предоставляет потомка по его порядковому номеру в списке родителя
 * \param index
 * \return
 */
Tree *Tree::childAt(int index) const
{
    Tree *child = firstChild();
    if(child && index < childCount())
    {
        int indexChild(0);
        while(child != 0)
        {
            if(indexChild == index) return child;

            indexChild++;
            child = child->nextSibling();
        }
    }
    return 0;
}

/*!
 * \brief Возвращает индекс потомка по его указателю
 * \param child
 * \return
 */
int Tree::indexChildOf(Tree *child) const
{
    if(child != 0 )
    {
        Tree *childNode = firstChild();
        int indexChild(0);
        while(childNode != 0)
        {
            if(childNode == child) return indexChild;

            indexChild++;
            childNode = childNode->nextSibling();
        }
    }

    return 0;
}

/*!
 * \brief Устанавливает родителя
 * \param parent
 */
void Tree::setParent(Tree *parent)
{
    m_parent = parent;
}

/*!
 * \brief Устанавливает первого потомка
 * \param firstChild
 */
void Tree::setFirstChild(Tree *firstChild)
{
    m_firstChild = firstChild;
}

/*!
 * \brief Устанавливает последнего потомка
 * \param lastChild
 */
void Tree::setLastChild(Tree *lastChild)
{
    m_lastChild = lastChild;
}

/*!
 * \brief Устанавливает следующего брата
 * \param nextSibling
 */
void Tree::setNextSibling(Tree *nextSibling)
{
    m_nextSibling = nextSibling;
}

/*!
 * \brief Устанавливает предидужего брата
 * \param previousSibling
 */
void Tree::setPreviousSibling(Tree *previousSibling)
{
    m_previousSibling = previousSibling;
}

/*!
 * \brief Копирует узел
 * \param other Узел который будет скопирован в текущий
 */
void Tree::copy(const Tree *other)
{
    Tree* child = other->firstChild();
    while(child != 0)
    {
        Tree *newChild = new Tree(*child);
        appendChild(newChild);
        child = child->nextSibling();
    }
}

/*!
 * \brief Добавляет потомка в конец списка
 * \param node
 */
void Tree::appendChild(Tree *node)
{
    if(node == 0 || node == this) return;

    node->setParent(this);
    m_childCount++;

    if(firstChild() == 0)
    {
        setFirstChild(node);
        setLastChild(node);
    }
    else
    {
        node->setPreviousSibling(lastChild());
        lastChild()->setNextSibling(node);

        setLastChild(node);
    }
}

/*!
 * \brief Добавляет брата по средствам родителя.
 *
 *  Корневой элемен не может иметь братьев.
 * \param node
 */
void Tree::appendSibling(Tree *node)
{
    if(parent() != 0)
    {
       parent()->appendChild(node);
    } else qDebug()<<tr("У корневого элемента не может быть братьев");
}



/*!
 * \brief Удалет узел из списка потомков своего родителя. Настраивет указатели соседних узлов и родителя.
 *
 *  Если у узла нет родителя то он считается одиноким. В этом случае функция ни чего не делает.
 */
void Tree::remove()
{
    if(parent() != 0 )
    {
        //Настраиваем ссылки родителя
        if(previousSibling() == 0 && nextSibling() != 0 ) //В начале
        {
            nextSibling()->setPreviousSibling(0);
            parent()->setFirstChild(nextSibling());
        } else if(previousSibling() != 0 && nextSibling() == 0 ) //В конце
        {
            previousSibling()->setNextSibling(0);
            parent()->setLastChild(previousSibling());
        } else  if(previousSibling() != 0 && nextSibling() != 0) //В середине
        {
            previousSibling()->setNextSibling(nextSibling());
        } else if(previousSibling() == 0 && nextSibling() == 0) //Один
        {
            parent()->setFirstChild(0);
            parent()->setLastChild(0);
        }
    }
}

/*!
 * \brief Удаляет всех потомков узла.
 */
void Tree::removeChildren()
{
    while(firstChild() != 0)
    {
        Tree *child = firstChild();
        DELETE_OBJECT(child); //Удаляем элемент
    }
}
