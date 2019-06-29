#include "idatabaseerror.h"

IDatabaseError::IDatabaseError() : std::shared_ptr<IDatabaseErrorStruct>()
{

}

IDatabaseError::IDatabaseError(IDatabaseSession *session)
    : std::shared_ptr<IDatabaseErrorStruct>(new IDatabaseErrorStruct())
{
    this->get()->m_session   = session;
    this->get()->m_sqlError  = QSqlError();
    this->get()->m_isHandled = true;
    this->get()->m_done      = nullptr;
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
        this->get()->m_isHandled = true;
    } else {
        this->get()->m_isHandled = false;
    }
}

void IDatabaseError::setHandlerDone(IHandlerDone done)
{
    if (this->get() == nullptr)
        return;

    this->get()->m_done = done;
    if (!this->get()->m_isHandled && done != nullptr) {
        this->get()->m_done(*this);
        this->get()->m_isHandled = true;
    }
}

IHandlerDone IDatabaseError::handlerDone()
{
    return (this->get() == nullptr) ? nullptr : this->get()->m_done;
}