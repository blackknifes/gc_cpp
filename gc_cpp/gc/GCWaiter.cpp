#include "GCWaiter.h"

GCWaiter::GCWaiter()
{
    ::InitializeConditionVariable(&m_condition);
}

GCWaiter::~GCWaiter() 
{
    notifyAll();
}

bool GCWaiter::wait(GCLocker* pLocker, DWORD timeout /*= INFINITE*/) const
{
    return ::SleepConditionVariableCS(&m_condition, &pLocker->m_section, timeout);
}

void GCWaiter::notify() const
{
    ::WakeConditionVariable(&m_condition);
}

void GCWaiter::notifyAll() const
{
    ::WakeAllConditionVariable(&m_condition);
}
