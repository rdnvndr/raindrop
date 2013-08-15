#include "querysqlwidget.h"
#include <QtGui>

QuerySqlWidget::QuerySqlWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    m_model = new QSqlQueryModel();

    tableView->setModel(m_model);

    connect(toolButtonRun,SIGNAL(clicked()),this,SLOT(runQuery()));
}

void QuerySqlWidget::runQuery(){

    //model->setQuery("SELECT * FROM operation3");
    m_model->setQuery(plainQueryEdit->toPlainText());
    if (m_model->lastError().isValid()){
        plainLogEdit->setPlainText(m_model->lastError().text());
        tabWidget->setCurrentIndex(1);
    }
    else{

        if (m_model->rowCount()<1)
            tabWidget->setCurrentIndex(1);
        else{
            tableView->show();
            tabWidget->setCurrentIndex(0);
        }
        if (m_model->lastError().text().count())
            plainLogEdit->setPlainText(tr("Запрос успешно выполнен"));
        else
            plainLogEdit->setPlainText(m_model->lastError().text());
    }

}
