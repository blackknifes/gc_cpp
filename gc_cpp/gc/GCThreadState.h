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

    HANDLE m_hThread;                          //�߳̾��
    GCScope* m_firstScope;                     //��ͷ����
    GCScope* m_lastScope;                      //��β����
    GCDelayConstruct<GCScope> m_rootScope;     //����
    GCLocker m_locker;                         //��ȫ����
    GCWaiter m_safePointWaiter;                //��ȫ��ȴ���
    GCWaiter* m_gcWaiter;                      // gc�ȴ���
    std::list<GarbageCollection*> m_garbages;  //�ȴ��ռ�������
    std::atomic<bool> m_safePoint;
    bool m_pauseRequested;
};
#endif