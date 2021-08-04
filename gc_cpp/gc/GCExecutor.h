#ifndef __GCEXECUTOR_H__
#define __GCEXECUTOR_H__
#include "GarbageCollected.h"

enum GCPhase
{
    GC_PHASE_SCAN,  //扫描阶段
    GC_PHASE_MARK,  //标记阶段
    GC_PHASE_SWEEP  //清理阶段
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
    GCPhase m_phase;  //当前阶段
};
#endif