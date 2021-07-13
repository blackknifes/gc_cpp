#ifndef __GCTHREADSTATE_H__
#define __GCTHREADSTATE_H__
#include <Windows.h>

#include <atomic>
#include <list>

#include "GCDelayConstruct.h"
#include "GCLocker.h"
#include "GCScope.h"
#include "GCStopFlag.h"
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

    void addGarbage(GarbageCollection* pGarbage);
    bool isOnSafePoint() const;

private:
    friend class GCScope;
    friend class GCManager;

    HANDLE m_hThread;                //Ïß³Ì¾ä±ú
    const GCStopFlag* m_gcStopFlag;  // gcµÈ´ýÆ÷
    void** m_stackLow;               // ¶ÑÕ»¶¥
    void** m_stackHigh;             // ¶ÑÕ»µ×

    std::list<GarbageCollection*> m_garbages;  //µÈ´ýÊÕ¼¯µÄÀ¬»ø
    std::atomic<bool> m_safePoint;
};
#endif