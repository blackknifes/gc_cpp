#ifndef __GCTHREADSTATE_H__
#define __GCTHREADSTATE_H__
#include <Windows.h>

#include <atomic>
#include <list>
#include <vector>

#include "GCLocker.h"
#include "GCScope.h"
#include "GCStopTheWorld.h"
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

    void enterScope(GCScope* pScope);
    void leaveScope();

    GCScope* getScope() const;

private:
    friend class GCManager;
    template<typename _Ty>
    friend class GCPersist;

    typedef GarbageCollected* (*PFN_Cast)(void*);
    void addRoot(void** ppAddress, PFN_Cast cast);
    void popRoot();

    HANDLE m_hThread;                      //线程句柄
    DWORD m_threadId;                      //线程id
    const GCStopTheWorld* m_stopTheWorld;  // gc等待器
    std::vector<std::pair<void**, PFN_Cast>> m_roots;

    GCScope* m_scope;

    std::list<GarbageCollected*> m_garbages;        //等待收集的垃圾
    std::atomic<bool> m_safePoint;
};
#endif