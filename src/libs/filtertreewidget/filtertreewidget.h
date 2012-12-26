#ifndef FILTERTREEWIDGET_H
#define FILTERTREEWIDGET_H

#include "filtertreewidgetglobal.h"
#include <QTreeWidget>

//! Класс дерева с фильтром
/*! Класс дерева с контекстной фильтрацией узлов
    предназначен для фильтрации отображаемых узлов
    по набраному в текстовом поле тексту
*/
class FILTERTREEWIDGETLIB FilterTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    //! Координата курсора мыши
    QPoint m_ptDragPos;

    //! Конструктор дерева с фильтром
    explicit FilterTreeWidget(QWidget *parent = 0);

    //! Событие по нажатию левой кнопки мыши
    virtual void mousePressEvent(QMouseEvent* pe);

    //! Событие по перемещению мыши
    virtual void mouseMoveEvent(QMouseEvent* pe);

    //! Начало перемещение узла
    void  startDrag();

    //! Окончание перемещения узла
    void dragEnterEvent(QDragEnterEvent *event);

    //! Проверка корректности события
    bool checkFormat(QDropEvent *ev);

    //! Событие перемещения узла
    void dragMoveEvent(QDragMoveEvent *ev);

public slots:

    //! Слот для отображения всех узлов дерева
    void showAllItem();

    //! Слот для фильтрации подузлов указанного узла по заданному тексту
    bool searchShowItem(QString text,QTreeWidgetItem* item);

    //! Слот для фильтрации узлов дерева по заданному тексту
    void searchShowAllItem(QString text);

signals:
    //! Сигнал отображения контекстного меню
    void contextMenuSignal(QContextMenuEvent *event);

protected:
    //! Вызов сигнала отображения контекстного меню
    void contextMenuEvent(QContextMenuEvent *event);

};

#endif
