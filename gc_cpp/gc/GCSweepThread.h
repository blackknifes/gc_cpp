#ifndef __GCSWEEPTHREAD_H__
#define __GCSWEEPTHREAD_H__
#include <vector>

#include "GCThread.h"
#include "GCWaiter.h"
#include "GarbageCollected.h"

class GCSweepThread : public GCThread
{
public:
    GCSweepThread();

    void run() override;

    void addGarbageList(std::vector<GarbageCollected*>& sweepList);

    void setStop();

    void sweep();
private:
    void doSweep();

    std::vector<GarbageCollected*> m_willSweepList;
    std::vector<GarbageCollected*> m_currentSweepList;
    GCLocker m_locker;
    GCWaiter m_waiter;
    bool m_runFlag;
    bool m_needSweep;
};
#endif