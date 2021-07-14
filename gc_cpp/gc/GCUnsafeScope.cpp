#include "GCUnsafeScope.h"

#include "GCThreadState.h"

GCUnsafeScope::GCUnsafeScope()
{
    m_threadState = GCThreadState::GetCurrent();
    if (m_threadState) m_threadState->leaveSafePoint();
}

GCUnsafeScope::~GCUnsafeScope()
{
    if (m_threadState) m_threadState->enterSafePoint();
}
