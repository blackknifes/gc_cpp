#ifndef __GCTHREADSTATE_H__
#define __GCTHREADSTATE_H__
#include <Windows.h>

#include <atomic>
#include <list>

#include "GCDelayConstruct.h"
#include "GCLocker.h"
#include "GCScope.h"
#include "GCWaiter.h"

class GarbageCollection;

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
    void pause(GCWaiter* pGcWaiter);
    void resume();

    void addGarbage(GarbageCollection* pGarbage);

    GCScope* getScope() const;

    bool isOnSafePoint() const;

private:
    friend class GCScope;
    friend class GCManager;

    HANDLE m_hThread;                          //线程句柄
    GCScope* m_firstScope;                     //开头根域
    GCScope* m_lastScope;                      //结尾根域
    GCDelayConstruct<GCScope> m_rootScope;     //根域
    GCLocker m_locker;                         //安全点锁
    GCWaiter m_safePointWaiter;                //安全点等待器
    GCWaiter* m_gcWaiter;                      // gc等待器
    std::list<GarbageCollection*> m_garbages;  //等待收集的垃圾
    std::atomic<bool> m_safePoint;
    bool m_pauseRequested;
};
#endif