#ifndef __GCTHREADSTATE_H__
#define __GCTHREADSTATE_H__
#include <Windows.h>

#include <atomic>
#include <list>
#include <vector>

#include "GCDelayConstruct.h"
#include "GCLocker.h"
#include "GCStopFlag.h"
#include "GCWaiter.h"

class GarbageCollected;

class GCThreadState
{
public:
    static GCThreadState* GetCurrent();

    GCThreadState();
    ~GCThreadState();

    DWORD getThreadId() const;

    void enterSafePoint();
    void leaveSafePoint();
    void waitEnterSafePoint();

    void addGarbage(GarbageCollected* pGarbage);
    bool isOnSafePoint() const;

private:
    friend class GCManager;
    template<typename _Ty>
    friend class GCPersist;

    typedef GarbageCollected* (*PFN_Cast)(void*);
    void addRoot(void** ppAddress, PFN_Cast cast);
    void popRoot();

    HANDLE m_hThread;                //线程句柄
    const GCStopFlag* m_gcStopFlag;  // gc等待器
    std::vector<std::pair<void**, PFN_Cast>> m_roots;

    std::list<GarbageCollected*> m_garbages;  //等待收集的垃圾
    std::atomic<bool> m_safePoint;
};
#endif