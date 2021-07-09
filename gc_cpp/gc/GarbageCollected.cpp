#include "GarbageCollected.h"

#include "GCScope.h"
#include "GCThreadState.h"

GarbageCollection::GarbageCollection() : m_mark(false), m_pin(true)
{
    GCThreadState* pThreadState = GCThreadState::GetCurrent();
    pThreadState->leaveSafePoint();
    GCScope::GetCurrent()->addGarbage(this);
    pThreadState->addGarbage(this);
    pThreadState->enterSafePoint();
}

GarbageCollection::~GarbageCollection() {}

void GarbageCollection::gcTrace(GCVisitor* visitor) {}

GarbageCollection* GarbageCollection::getObjectPointer() const
{
    return const_cast<GarbageCollection*>(this);
}

void GarbageCollection::gcMark() const
{
    m_mark = true;
}

void GarbageCollection::gcUnmark() const
{
    m_mark = false;
}

bool GarbageCollection::isGcMarked() const
{
    return m_mark;
}

void GarbageCollection::gcPin() const
{
    m_pin = true;
}

void GarbageCollection::gcUnpin() const
{
    m_pin = false;
}

bool GarbageCollection::isPined() const
{
    return m_pin;
}
