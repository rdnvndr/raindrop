#ifndef QUERYSQLWIDGET_H
#define QUERYSQLWIDGET_H

#include "ui_querysqlwidget.h"
#include <QtSql>

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
};

#endif // QUERYSQLWIDGET_H
