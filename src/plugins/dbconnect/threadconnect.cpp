#include "threadconnect.h"

#include <QSqlDatabase>
#include <QSqlError>

namespace RTPTechGroup {
namespace DbConnect {

ThreadConnect::ThreadConnect(QObject *parent): QThread(parent)
{

}

void ThreadConnect::run()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.open()) {
        QSqlError err = db.lastError();
        if (err.type() != QSqlError::NoError)
            emit finishConnect(err.text());
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    } else
        emit finishConnect("");
}

}}
