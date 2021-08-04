#include "GarbageCollected.h"

#include "GCManager.h"
#include "GCScope.h"
#include "GCThreadState.h"

void GarbageCollectedOperation::setColor(GarbageCollected* pObject, GCMarkColor color)
{
    pObject->m_gcValue.color = color;
}

GCMarkColor GarbageCollectedOperation::getColor(GarbageCollected* pObject)
{
    return pObject->m_gcValue.color;
}

void GarbageCollectedOperation::setDirty(GarbageCollected* pObject, bool dirty)
{
    pObject->m_gcValue.dirty = dirty;
}

bool GarbageCollectedOperation::isDirty(GarbageCollected* pObject)
{
    return pObject->m_gcValue.dirty;
}

void GarbageCollectedOperation::addAge(GarbageCollected* pObject)
{
    ++pObject->m_gcValue.age;
}

size_t GarbageCollectedOperation::getAge(GarbageCollected* pObject)
{
    return pObject->m_gcValue.age;
}

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