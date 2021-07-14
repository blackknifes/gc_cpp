#include "GarbageCollected.h"

#include "GCThreadState.h"

GarbageCollected::GarbageCollected() : m_mark(false)
{
    GCThreadState* pThreadState = GCThreadState::GetCurrent();
    pThreadState->leaveSafePoint();
    pThreadState->addGarbage(this);
    pThreadState->enterSafePoint();
}

GarbageCollected::~GarbageCollected() {}

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