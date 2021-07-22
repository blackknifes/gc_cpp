#include "GCScope.h"

#include "GCThreadState.h"

GCScope* GCScope::GetCurrent()
{
    return GCThreadState::GetCurrent()->getScope();
}

GCScope::GCScope() : m_next(nullptr)
{
    GCThreadState::GetCurrent()->enterScope(this);
}

GCScope::~GCScope()
{
    GCThreadState::GetCurrent()->leaveScope();
}

GCScope* GCScope::pre() const
{
    return m_pre;
}

GCScope* GCScope::next() const
{
    return m_next;
}

void GCScope::addObject(GarbageCollected* pObject)
{
    m_roots.push_back(pObject);
}

void GCScope::visit(GCVisitor* visitor)
{
    for (GarbageCollected* pObject : m_roots) visitor->visit(pObject);
}
