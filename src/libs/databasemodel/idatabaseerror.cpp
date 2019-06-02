#include "idatabaseerror.h"

IDatabaseError::IDatabaseError(const QSqlError &err, IDatabaseSession *s)
{
    m_sqlError = err;
    m_session = s;
}

IDatabaseSession *IDatabaseError::session() const
{
    return m_session;
}

bool IDatabaseError::isValid() const
{
    return m_sqlError.isValid();
}
