#include "GCVisitor.h"

#include "GarbageCollected.h"

void GCVisitor::visit(GarbageCollection* pGarbage)
{
    if (!pGarbage || pGarbage->isGcMarked()) return;
    pGarbage->gcMark();
    pGarbage->gcTrace(this);
}
