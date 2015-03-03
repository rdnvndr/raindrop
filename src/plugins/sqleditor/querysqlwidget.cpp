#include "querysqlwidget.h"
#include <QtGui>

namespace RTPTechGroup {
namespace SqlEditor {

QuerySqlWidget::QuerySqlWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    m_model = new QSqlQueryModel();

    tableView->setModel(m_model);
    m_sqlHighlighter = new SqlHighlighter(plainQueryEdit->document());

    connect(toolButtonRun,SIGNAL(clicked()),this,SLOT(runQuery()));
}

void QuerySqlWidget::runQuery() {
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

}}
