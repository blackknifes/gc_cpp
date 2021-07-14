#include "GCVisitor.h"

#include "GarbageCollected.h"

void GCVisitor::visit(GarbageCollected* pGarbage)
{
    if (!pGarbage || pGarbage->isGcMarked()) return;
    pGarbage->gcMark();
    pGarbage->gcTrace(this);
}
