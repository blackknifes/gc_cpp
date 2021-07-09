#include "GCUnsafeScope.h"

#include "GCThreadState.h"

GCUnsafeScope::GCUnsafeScope()
{
    m_threadState = GCThreadState::GetCurrent();
    m_threadState->leaveSafePoint();
}

GCUnsafeScope::~GCUnsafeScope()
{
    m_threadState->enterSafePoint();
}
