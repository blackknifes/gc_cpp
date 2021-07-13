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

    HANDLE m_hThread;                //�߳̾��
    const GCStopFlag* m_gcStopFlag;  // gc�ȴ���
    void** m_stackLow;               // ��ջ��
    void** m_stackHigh;             // ��ջ��

    std::list<GarbageCollection*> m_garbages;  //�ȴ��ռ�������
    std::atomic<bool> m_safePoint;
};
#endif