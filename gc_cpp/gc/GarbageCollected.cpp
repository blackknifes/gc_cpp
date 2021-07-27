#include "GarbageCollected.h"

#include "GCManager.h"
#include "GCScope.h"
#include "GCThreadState.h"

GarbageCollected::GarbageCollected()
{
    memset(&m_gcValue, 0, sizeof(m_gcValue));
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

void GarbageCollected::gcSetColor(GCMarkColor color /*= MARK_BLACK*/) const
{
    m_gcValue.color = color;
}

GCMarkColor GarbageCollected::getGcMarkColor() const
{
    return m_gcValue.color;
}

void GarbageCollected::addGCAge() const
{
    ++m_gcValue.age;
}

size_t GarbageCollected::getGCAge() const
{
    return m_gcValue.age;
}
