#include "idatabaseerror.h"

IDatabaseError::IDatabaseError() : std::shared_ptr<IDatabaseErrorStruct>()
{

}

IDatabaseError::IDatabaseError(IDatabaseSession *session)
    : std::shared_ptr<IDatabaseErrorStruct>(new IDatabaseErrorStruct())
{
    this->get()->m_session = session;
    this->get()->m_sqlError = QSqlError();
    this->get()->m_isHandledDone = true;
    this->get()->m_done = nullptr;
}

IDatabaseSession *IDatabaseError::session() const
{
    return (this->get() == nullptr) ? nullptr : this->get()->m_session;
}

bool IDatabaseError::isValid() const
{
    return (this->get() == nullptr) ? true : this->get()->m_sqlError.isValid();
}

QSqlError IDatabaseError::sqlError() const
{
    return (this->get() == nullptr) ? QSqlError() : this->get()->m_sqlError;
}

void IDatabaseError::finish(const QSqlError &err)
{
    if (this->get() == nullptr)
        return;

    this->get()->m_sqlError = err;
    if (this->get()->m_done != nullptr) {
        this->get()->m_done(*this);
        this->get()->m_isHandledDone = true;
    } else {
        this->get()->m_isHandledDone = false;
    }
}

void IDatabaseError::setHandlerDone(IHandlerFunction done)
{
    if (this->get() == nullptr)
        return;

    this->get()->m_done = done;
    if (!this->get()->m_isHandledDone && done != nullptr) {
        this->get()->m_done(*this);
        this->get()->m_isHandledDone = true;
    }
}
