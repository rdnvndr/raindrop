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
    void undoCommandAdd();


private:
    QSqlQueryModel    *m_model;
    SqlHighlighter    *m_sqlHighlighter;
    QUndoStack        *m_undoStack;
    IUndoGroup        *m_undoGroup;
    IClipboardStack   *m_clipboardStack;
    TextClipboardItem *m_clipboardItem;
};

}}

#endif // QUERYSQLWIDGET_H
