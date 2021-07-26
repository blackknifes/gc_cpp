#ifndef __GCMARKTHREAD_H__
#define __GCMARKTHREAD_H__
#include "GCLocker.h"
#include "GCManager.h"
#include "GCThread.h"
#include "GCWaiter.h"

// gc标记线程
class GCMarkThread : public GCThread
{
public:
    GCMarkThread(GCManager* pManager);
    void run() override;

    void setStop();

    void gc(GCManager::GCType gcType = GCManager::GC_MINOR);

private:
    void doMark();
    GCManager* m_manager;

    GCLocker m_locker;
    GCWaiter m_waiter;

    bool m_needGC;
    bool m_runFlag;
    GCManager::GCType m_gcType;
};
#endif