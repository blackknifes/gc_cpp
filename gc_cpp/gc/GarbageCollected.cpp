#include "GarbageCollected.h"

#include "GCManager.h"
#include "GCScope.h"
#include "GCThreadState.h"

std::atomic<size_t> s_count = 0;

GarbageCollected::GarbageCollected() : m_mark(false)
{
    GCThreadState* pThreadState = GCThreadState::GetCurrent();
    GCScope* pScope = GCScope::GetCurrent();
    pThreadState->leaveSafePoint();
    pThreadState->addGarbage(this);
    if (pScope) pScope->addObject(this);
    pThreadState->enterSafePoint();
    if (++s_count > 102400) GCManager::GetGlobal()->gc();
}

GarbageCollected::~GarbageCollected()
{
    --s_count;
}

void GarbageCollected::gcTrace(GCVisitor* visitor) {}

GarbageCollected* GarbageCollected::getObjectPointer() const
{
    return const_cast<GarbageCollected*>(this);
}

void GarbageCollected::gcMark() const
{
    m_mark = true;
}

void GarbageCollected::gcUnmark() const
{
    m_mark = false;
}

bool GarbageCollected::isGcMarked() const
{
    return m_mark;
}