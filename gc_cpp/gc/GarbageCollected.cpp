#include "GarbageCollected.h"

#include "GCManager.h"
#include "GCScope.h"
#include "GCThreadState.h"

GarbageCollected::GarbageCollected() : m_gcValue(0)
{
    GCThreadState* pThreadState = GCThreadState::GetCurrent();
    GCScope* pScope = GCScope::GetCurrent();
    pThreadState->leaveSafePoint();
    pThreadState->addGarbage(this);
    if (pScope) pScope->addObject(this);
    pThreadState->enterSafePoint();
    GCManager::GetGlobal()->addEden();
}

GarbageCollected::~GarbageCollected() {}

void GarbageCollected::gcTrace(GCVisitor* visitor) {}

GarbageCollected* GarbageCollected::getObjectPointer() const
{
    return const_cast<GarbageCollected*>(this);
}

void GarbageCollected::gcMark() const
{
    m_gcValue |= kMarkMask;
}

void GarbageCollected::gcUnmark() const
{
    m_gcValue &= ~kMarkMask;
}

bool GarbageCollected::isGcMarked() const
{
    return (m_gcValue & kMarkMask) != 0;
}

void GarbageCollected::addGCAge() const
{
    ++m_gcValue;
}

size_t GarbageCollected::getGCAge() const
{
    return m_gcValue & kAgeMask;
}
