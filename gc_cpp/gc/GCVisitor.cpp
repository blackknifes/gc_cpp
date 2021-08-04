#include "GCVisitor.h"

#include <conio.h>

#include "GarbageCollected.h"
#include "platform/GCPlatformAPI.h"
GCVisitorCounted::GCVisitorCounted(GCVisitor* pVisitor) : m_visitor(pVisitor), m_counter(0) {}

GCVisitorCounted::~GCVisitorCounted() {}

void GCVisitorCounted::visit(GarbageCollected* pGarbage)
{
    m_visitor->visit(pGarbage);
    ++m_counter;
}

size_t GCVisitorCounted::getVisitCount() const
{
    return m_counter;
}

void GCVisitorFull::visit(GarbageCollected* pGarbage)
{
    if (!pGarbage || GarbageCollectedOperation::getColor(pGarbage) != GC_MARK_BLACK) return;
    GarbageCollectedOperation::setColor(pGarbage, GC_MARK_BLACK);
    pGarbage->gcTrace(this);
}

GCVisitorIncrement::GCVisitorIncrement(uint64_t timeout)
    : m_couter(0),
      m_timeoutMicroSeconds(timeout),
      m_needReturn(false)
{
}

void GCVisitorIncrement::visit(GarbageCollected* pGarbage)
{
    //需要返回或者对象不为脏，直接跳出
    if (m_needReturn || !GarbageCollectedOperation::isDirty(pGarbage)) return;

    if (!pGarbage || GarbageCollectedOperation::getColor(pGarbage) != GC_MARK_BLACK) return;
    if (++m_couter > kCheckLoopCount &&
        GCPlatformAPI::GetPrecisionMicroSecond() > m_timeoutMicroSeconds)
    {
        m_needReturn = true;
    }
    else
    {
        GarbageCollectedOperation::setColor(pGarbage, GC_MARK_GRAY);
        pGarbage->gcTrace(this);
        if (!m_needReturn) GarbageCollectedOperation::setColor(pGarbage, GC_MARK_BLACK);
    }
}

bool GCVisitorIncrement::isFinish() const
{
    return !m_needReturn;
}
