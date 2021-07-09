#include "GCLocker.h"

GCLocker::GCLocker()
{
    ::InitializeCriticalSectionAndSpinCount(&m_section, 2000);
}

GCLocker::~GCLocker()
{
    ::DeleteCriticalSection(&m_section);
}

void GCLocker::lock() const
{
    ::EnterCriticalSection(&m_section);
}

void GCLocker::unlock() const
{
    ::LeaveCriticalSection(&m_section);
}
