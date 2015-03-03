#ifndef QUERYSQLWIDGET_H
#define QUERYSQLWIDGET_H

#include "ui_querysqlwidget.h"
#include "sqlhighlighter.h"
#include <QtSql>

namespace RTPTechGroup {
namespace SqlEditor {

//! Редактор SQL запросов

class QuerySqlWidget : public QWidget, private Ui::QuerySqlWidget
{
    Q_OBJECT

public:
    //! Конструктор
    explicit QuerySqlWidget(QWidget *parent = 0);

public slots:
    //! Слот для вызова выполнения SQL запроса
    void runQuery();

private:
    QSqlQueryModel* m_model;
    SqlHighlighter *m_sqlHighlighter;
};

}}

#endif // QUERYSQLWIDGET_H
