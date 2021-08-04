#ifndef __GCEXECUTOR_H__
#define __GCEXECUTOR_H__
#include "GarbageCollected.h"

enum GCPhase
{
    GC_PHASE_SCAN,  //ɨ��׶�
    GC_PHASE_MARK,  //��ǽ׶�
    GC_PHASE_SWEEP  //����׶�
};

class GCExecutor
{
public:
    GCExecutor();
    virtual ~GCExecutor();
    virtual void scanRoot() = 0;
    virtual void mark() = 0;
    virtual void sweep() = 0;

    void runOnce();

    GCPhase getPhase() const;

protected:
    void setPhase(GCPhase phase);

private:
    GCPhase m_phase;  //��ǰ�׶�
};
#endif