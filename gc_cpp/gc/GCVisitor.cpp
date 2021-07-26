#include "GCVisitor.h"

#include <conio.h>

#include "GarbageCollected.h"

void GCVisitor::visit(GarbageCollected* pGarbage)
{
    if (!pGarbage || pGarbage->isGcMarked()) return;
    pGarbage->gcMark();
    pGarbage->gcTrace(this);
}

GCVisitorCounted::GCVisitorCounted() : m_counter(0) {}

GCVisitorCounted::~GCVisitorCounted() {}

void GCVisitorCounted::visit(GarbageCollected* pGarbage)
{
    GCVisitor::visit(pGarbage);
    ++m_counter;
}

size_t GCVisitorCounted::getVisitCount() const
{
    return m_counter;
}
