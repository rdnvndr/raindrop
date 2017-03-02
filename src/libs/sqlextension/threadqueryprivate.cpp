#include "threadqueryprivate.h"

#include <QtCore/QThread>
#include <QtSql/QSqlRecord>

namespace RTPTechGroup {
namespace SqlExtension {

ThreadQueryPrivate::ThreadQueryPrivate(const QString &driverName,
                                 const QString &databaseName,
                                 const QString &hostName,
                                 int port, const
                                 QString &userName,
                                 const QString &password,
                                 const QString &query)
{
    QThread* curThread = QThread::currentThread();
    QSqlDatabase m_db = QSqlDatabase::addDatabase(
                driverName,
                QString("RTP0x%1").arg((qlonglong)curThread, 0, 16));
    m_db.setDatabaseName(databaseName);
    m_db.setHostName(hostName);
    m_db.setPort(port);
    m_db.setUserName(userName);
    m_db.setPassword(password);
    if (!m_db.open()) {
        emit error(m_db.lastError());
        emit executeDone(false);
    }

    if (query.isEmpty())
        m_query = new QSqlQuery(m_db);
    else
        m_query = new QSqlQuery(query, m_db);
}

ThreadQueryPrivate::~ThreadQueryPrivate()
{
    delete m_query;
    if (m_db.isOpen()) {
        m_db.close();
    }
    QSqlDatabase::removeDatabase(m_db.connectionName());
}

void ThreadQueryPrivate::bindValue(const QString &placeholder, const QVariant &val,
                                QSql::ParamType paramType)
{
    m_query->bindValue(placeholder, val, paramType);
}

void ThreadQueryPrivate::setNumericalPrecisionPolicy(
        QSql::NumericalPrecisionPolicy precisionPolicy)
{
    m_query->setNumericalPrecisionPolicy(precisionPolicy);
}

void ThreadQueryPrivate::setForwardOnly(bool forward)
{
    m_query->setForwardOnly(forward);
}

bool ThreadQueryPrivate::prepare(const QString &query)
{
    bool ret = m_query->prepare(query);
    if (!ret) {
        emit error(m_query->lastError());
        emit executeDone(ret);
    }
    return ret;
}

bool ThreadQueryPrivate::execute()
{
    bool ret = m_query->exec();
    if (!ret) emit error(m_query->lastError());
    emit executeDone(ret);
    return ret;
}

bool ThreadQueryPrivate::execute(const QString &query)
{
    bool ret = m_query->exec(query);
    if (!ret) emit error(m_query->lastError());
    emit executeDone(ret);
    return ret;
}

bool ThreadQueryPrivate::executeBatch(QSqlQuery::BatchExecutionMode mode)
{
    bool ret = m_query->execBatch(mode);
    emit executeDone(ret);
    return ret;
}

bool ThreadQueryPrivate::first()
{
    bool ret = m_query->first();
    emit changePosition(ret ? m_query->at() : -1);
    return ret;
}

bool ThreadQueryPrivate::next()
{
    bool ret = m_query->next();
    emit changePosition(ret ? m_query->at() : -1);
    return ret;
}

bool ThreadQueryPrivate::seek(int index, bool relative)
{
    bool ret = m_query->seek(index, relative);
    emit changePosition(ret ? m_query->at() : -1);
    return ret;
}

bool ThreadQueryPrivate::previous()
{
    bool ret = m_query->previous();
    emit changePosition(ret ? m_query->at() : -1);
    return ret;
}

bool ThreadQueryPrivate::last()
{
    bool ret = m_query->last();
    emit changePosition(ret ? m_query->at() : -1);
    return ret;
}

void ThreadQueryPrivate::fetchAll()
{
    QList<QSqlRecord> records;
    while (m_query->next())
    {
        records.append(m_query->record());
    }
    emit values(records);
}

void ThreadQueryPrivate::fetchOne()
{
    emit value(m_query->record());
}

void ThreadQueryPrivate::finish()
{
    m_query->finish();
}

void ThreadQueryPrivate::clear()
{
    m_query->clear();
}

}}
