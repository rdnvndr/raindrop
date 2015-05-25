#ifndef QUERYSQLWIDGET_H
#define QUERYSQLWIDGET_H

#include "ui_querysqlwidget.h"
#include "sqlhighlighter.h"

#include <QtSql>
#include <QUndoStack>
#include <QUndoGroup>

#include <iundogroup.h>
#include <iclipboardstack.h>

#include "textclipboarditem.h"

namespace RTPTechGroup {
namespace SqlEditor {

//! Редактор SQL запросов

class QuerySqlWidget : public QWidget, private Ui::QuerySqlWidget
{
    Q_OBJECT

public:
    //! Конструктор
    explicit QuerySqlWidget(QWidget *parent = 0);

    //! Деструктор
    ~QuerySqlWidget();

    //! Перехват события
    bool eventFilter(QObject *target, QEvent *event);

public slots:
    //! Слот для вызова выполнения SQL запроса
    void runQuery();

private slots:
    //! Обработка добавление команды в стек отмены
    void undoCommandAdd();


private:
    //! SQL модель
    QSqlQueryModel    *m_model;

    //! SQL подсветка синтаксиса в редакторе
    SqlHighlighter    *m_sqlHighlighter;

    //! Текущий стек отмены
    QUndoStack        *m_undoStack;

    //! Список стеков отмены
    IUndoGroup        *m_undoGroup;

    //! Стек буфера обмена
    IClipboardStack   *m_clipboardStack;

    //! Текущий элемент стека буфера обмены
    TextClipboardItem *m_clipboardItem;
};

}}

#endif // QUERYSQLWIDGET_H
