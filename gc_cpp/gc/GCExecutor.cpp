#include "GCExecutor.h"

GCExecutor::GCExecutor() : m_phase(GC_PHASE_MARK) {}

GCExecutor::~GCExecutor() {}

void GCExecutor::runOnce()
{
    switch (m_phase)
    {
    case GC_PHASE_SCAN:
        scanRoot();
        break;
    case GC_PHASE_MARK:
        mark();
        break;
    case GC_PHASE_SWEEP:
        sweep();
        break;
    default:
        break;
    }
}

GCPhase GCExecutor::getPhase() const
{
    return m_phase;
}

void GCExecutor::setPhase(GCPhase phase)
{
    m_phase = phase;
}
